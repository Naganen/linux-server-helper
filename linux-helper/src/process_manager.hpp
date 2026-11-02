#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include "json.hpp"

using json = nlohmann::json;

struct ProcessInfo {
    int pid = 0;
    std::string name;
    std::string user;
    std::string state;
    double cpu_percent = 0.0;
    unsigned long long memory_rss_kb = 0;
    double memory_percent = 0.0;
    unsigned long long total_time_ticks = 0;
    unsigned long long disk_read_bytes = 0;
    unsigned long long disk_write_bytes = 0;
};

struct ProcCpuSnapshot {
    unsigned long long ticks = 0;
    std::chrono::steady_clock::time_point time;
};

class ProcessManager {
private:
    static std::unordered_map<int, ProcCpuSnapshot>& get_prev_snapshots() {
        static std::unordered_map<int, ProcCpuSnapshot> snapshots;
        return snapshots;
    }

    static unsigned long long get_total_ram_kb() {
        std::ifstream file("/proc/meminfo");
        std::string line;
        while (std::getline(file, line)) {
            if (line.rfind("MemTotal:", 0) == 0) {
                std::istringstream ss(line.substr(9));
                unsigned long long total_kb = 0;
                if (ss >> total_kb) {
                    return total_kb;
                }
            }
        }
        return 0;
    }

    static double get_uptime_seconds() {
        std::ifstream file("/proc/uptime");
        double up = 0.0;
        if (file.is_open()) {
            file >> up;
        }
        return up;
    }

public:
    static std::string get_user_name(uid_t uid) {
        struct passwd* pw = getpwuid(uid);
        if (pw) {
            return std::string(pw->pw_name);
        }
        return std::to_string(uid);
    }

    static json get_process_list(size_t limit = 150) {
        json proc_array = json::array();
        DIR* dir = opendir("/proc");
        if (!dir) return proc_array;

        unsigned long long total_ram_kb = get_total_ram_kb();
        double uptime_sec = get_uptime_seconds();
        long clk_tck = sysconf(_SC_CLK_TCK);
        if (clk_tck <= 0) clk_tck = 100;
        long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
        if (num_cores <= 0) num_cores = 1;

        auto now = std::chrono::steady_clock::now();
        auto& prev_snapshots = get_prev_snapshots();
        std::unordered_map<int, ProcCpuSnapshot> curr_snapshots;

        struct dirent* entry;
        std::vector<ProcessInfo> list;

        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type != DT_DIR) continue;
            std::string dname(entry->d_name);
            if (dname.empty() || !std::all_of(dname.begin(), dname.end(), ::isdigit)) continue;

            int pid = std::stoi(dname);
            ProcessInfo p;
            p.pid = pid;

            // 1. Read /proc/[pid]/comm
            std::ifstream comm_file("/proc/" + dname + "/comm");
            if (comm_file.is_open()) {
                std::getline(comm_file, p.name);
            }

            // 2. Read /proc/[pid]/status
            std::ifstream status_file("/proc/" + dname + "/status");
            if (status_file.is_open()) {
                std::string line;
                while (std::getline(status_file, line)) {
                    if (line.rfind("State:", 0) == 0) {
                        p.state = line.substr(7, 1);
                    } else if (line.rfind("Uid:", 0) == 0) {
                        std::istringstream ss(line.substr(5));
                        uid_t real_uid;
                        if (ss >> real_uid) {
                            p.user = get_user_name(real_uid);
                        }
                    } else if (line.rfind("VmRSS:", 0) == 0) {
                        std::istringstream ss(line.substr(7));
                        ss >> p.memory_rss_kb;
                    }
                }
            }

            if (total_ram_kb > 0 && p.memory_rss_kb > 0) {
                p.memory_percent = (100.0 * p.memory_rss_kb) / total_ram_kb;
            }

            // 3. Read /proc/[pid]/stat for CPU calculation
            std::ifstream stat_file("/proc/" + dname + "/stat");
            if (stat_file.is_open()) {
                std::string line;
                if (std::getline(stat_file, line)) {
                    size_t close_paren = line.rfind(')');
                    if (close_paren != std::string::npos && close_paren + 2 < line.size()) {
                        std::istringstream ss(line.substr(close_paren + 2));
                        std::string state_char;
                        int ppid, pgrp, session, tty_nr, tpgid;
                        unsigned int flags;
                        unsigned long minflt, cminflt, majflt, cmajflt;
                        unsigned long utime = 0, stime = 0;
                        long cutime, cstime, priority, nice_val, num_threads, itrealvalue;
                        unsigned long long starttime = 0;

                        if (ss >> state_char >> ppid >> pgrp >> session >> tty_nr >> tpgid
                               >> flags >> minflt >> cminflt >> majflt >> cmajflt
                               >> utime >> stime >> cutime >> cstime >> priority >> nice_val
                               >> num_threads >> itrealvalue >> starttime) {
                            p.total_time_ticks = utime + stime;

                            curr_snapshots[pid] = { p.total_time_ticks, now };

                            // Calculate instant CPU % if previous snapshot exists
                            auto it = prev_snapshots.find(pid);
                            if (it != prev_snapshots.end()) {
                                double elapsed = std::chrono::duration<double>(now - it->second.time).count();
                                if (elapsed > 0.05 && p.total_time_ticks >= it->second.ticks) {
                                    unsigned long long diff_ticks = p.total_time_ticks - it->second.ticks;
                                    double cpu_usage = (static_cast<double>(diff_ticks) / clk_tck) / elapsed * 100.0;
                                    p.cpu_percent = std::min(100.0 * num_cores, std::max(0.0, cpu_usage));
                                }
                            } else {
                                // Fallback to process lifetime CPU average
                                double proc_elapsed = uptime_sec - (static_cast<double>(starttime) / clk_tck);
                                if (proc_elapsed > 0.1) {
                                    double total_sec = static_cast<double>(p.total_time_ticks) / clk_tck;
                                    p.cpu_percent = std::min(100.0 * num_cores, std::max(0.0, (total_sec / proc_elapsed) * 100.0));
                                }
                            }
                        }
                    }
                }
            }

            // 4. Read /proc/[pid]/io for disk usage
            std::ifstream io_file("/proc/" + dname + "/io");
            if (io_file.is_open()) {
                std::string line;
                while (std::getline(io_file, line)) {
                    if (line.rfind("read_bytes:", 0) == 0) {
                        std::istringstream ss(line.substr(11));
                        ss >> p.disk_read_bytes;
                    } else if (line.rfind("write_bytes:", 0) == 0) {
                        std::istringstream ss(line.substr(12));
                        ss >> p.disk_write_bytes;
                    }
                }
            }

            if (!p.name.empty()) {
                list.push_back(p);
            }
        }
        closedir(dir);

        prev_snapshots = std::move(curr_snapshots);

        // Sort by CPU percent descending, then by memory
        std::sort(list.begin(), list.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
            if (std::abs(a.cpu_percent - b.cpu_percent) > 0.1) {
                return a.cpu_percent > b.cpu_percent;
            }
            return a.memory_rss_kb > b.memory_rss_kb;
        });

        if (limit > 0 && list.size() > limit) {
            list.resize(limit);
        }

        for (const auto& item : list) {
            json item_j;
            item_j["pid"] = item.pid;
            item_j["name"] = item.name;
            item_j["user"] = item.user;
            item_j["state"] = item.state;
            item_j["cpu_percent"] = std::round(item.cpu_percent * 10.0) / 10.0;
            item_j["memory_kb"] = item.memory_rss_kb;
            item_j["memory_mb"] = item.memory_rss_kb / 1024.0;
            item_j["memory_percent"] = item.memory_percent;
            item_j["cpu_ticks"] = item.total_time_ticks;
            item_j["disk_read_bytes"] = item.disk_read_bytes;
            item_j["disk_write_bytes"] = item.disk_write_bytes;
            item_j["disk_total_bytes"] = item.disk_read_bytes + item.disk_write_bytes;
            proc_array.push_back(item_j);
        }

        return proc_array;
    }

    static bool kill_process(int pid, int signal_num = 15) {
        if (pid <= 1) return false;
        return (kill(pid, signal_num) == 0);
    }
};
