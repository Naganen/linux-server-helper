#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <dirent.h>
#include "json.hpp"

using json = nlohmann::json;

struct CpuStats {
    unsigned long long user = 0;
    unsigned long long nice = 0;
    unsigned long long system = 0;
    unsigned long long idle = 0;
    unsigned long long iowait = 0;
    unsigned long long irq = 0;
    unsigned long long softirq = 0;
    unsigned long long steal = 0;
};

class MetricsCollector {
private:
    CpuStats prev_cpu_;
    std::chrono::steady_clock::time_point prev_time_;
    unsigned long long prev_rx_bytes_ = 0;
    unsigned long long prev_tx_bytes_ = 0;

    static CpuStats read_cpu_raw() {
        CpuStats stats;
        std::ifstream file("/proc/stat");
        std::string line;
        if (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string cpu;
            ss >> cpu >> stats.user >> stats.nice >> stats.system >> stats.idle
               >> stats.iowait >> stats.irq >> stats.softirq >> stats.steal;
        }
        return stats;
    }

    static std::string format_sensor_friendly_name(const std::string& raw) {
        std::string s = raw;
        // Trim whitespace
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);

        std::string lower = s;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower.find("x86_pkg_temp") != std::string::npos || lower.find("pkg_temp") != std::string::npos || lower.find("cpu_thermal") != std::string::npos) {
            return "İşlemci (CPU Paketi)";
        }
        if (lower.rfind("bat", 0) == 0 || lower.find("battery") != std::string::npos) {
            return "Batarya (Pil)";
        }
        if (lower.find("acpitz") != std::string::npos) {
            return "Anakart (ACPI)";
        }
        if (lower.find("pch") != std::string::npos) {
            return "Yonga Seti (PCH)";
        }
        if (lower.find("composite") != std::string::npos || lower.find("nvme") != std::string::npos) {
            return "NVMe SSD Depolama";
        }
        if (lower.find("iwlwifi") != std::string::npos || lower.find("wifi") != std::string::npos || lower.find("wlan") != std::string::npos) {
            return "Kablosuz Ağ (Wi-Fi)";
        }
        if (lower.find("core") != std::string::npos) {
            return "CPU Çekirdeği";
        }
        if (lower.find("amdgpu") != std::string::npos || lower.find("nouveau") != std::string::npos || lower.find("nvidia") != std::string::npos) {
            return "Ekran Kartı (GPU)";
        }
        return s;
    }

public:
    MetricsCollector() {
        prev_cpu_ = read_cpu_raw();
        prev_time_ = std::chrono::steady_clock::now();
        get_net_io(prev_rx_bytes_, prev_tx_bytes_);
    }

    double get_cpu_usage() {
        CpuStats curr = read_cpu_raw();
        unsigned long long prev_idle = prev_cpu_.idle + prev_cpu_.iowait;
        unsigned long long curr_idle = curr.idle + curr.iowait;

        unsigned long long prev_non_idle = prev_cpu_.user + prev_cpu_.nice + prev_cpu_.system +
                                           prev_cpu_.irq + prev_cpu_.softirq + prev_cpu_.steal;
        unsigned long long curr_non_idle = curr.user + curr.nice + curr.system +
                                           curr.irq + curr.softirq + curr.steal;

        unsigned long long prev_total = prev_idle + prev_non_idle;
        unsigned long long curr_total = curr_idle + curr_non_idle;

        unsigned long long total_diff = curr_total - prev_total;
        unsigned long long idle_diff = curr_idle - prev_idle;

        double usage = 0.0;
        if (total_diff > 0) {
            usage = 100.0 * static_cast<double>(total_diff - idle_diff) / static_cast<double>(total_diff);
        }

        prev_cpu_ = curr;
        return usage;
    }

    static json get_ram_usage() {
        json mem;
        std::ifstream file("/proc/meminfo");
        std::string line;
        unsigned long long total = 0, available = 0, free = 0, buffers = 0, cached = 0;
        unsigned long long swap_total = 0, swap_free = 0;

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string key;
            unsigned long long val;
            std::string unit;
            ss >> key >> val >> unit;

            if (key == "MemTotal:") total = val * 1024;
            else if (key == "MemAvailable:") available = val * 1024;
            else if (key == "MemFree:") free = val * 1024;
            else if (key == "Buffers:") buffers = val * 1024;
            else if (key == "Cached:") cached = val * 1024;
            else if (key == "SwapTotal:") swap_total = val * 1024;
            else if (key == "SwapFree:") swap_free = val * 1024;
        }

        unsigned long long used = (available > 0) ? (total - available) : (total - free - buffers - cached);
        double percent = (total > 0) ? (100.0 * used / total) : 0.0;

        unsigned long long swap_used = swap_total - swap_free;
        double swap_percent = (swap_total > 0) ? (100.0 * swap_used / swap_total) : 0.0;

        mem["total"] = total;
        mem["used"] = used;
        mem["available"] = available;
        mem["free"] = free;
        mem["percent"] = percent;
        mem["swap_total"] = swap_total;
        mem["swap_used"] = swap_used;
        mem["swap_percent"] = swap_percent;

        return mem;
    }

    static json get_disk_usage(const std::string& path = "/") {
        json disk;
        struct statvfs stat;
        if (statvfs(path.c_str(), &stat) == 0) {
            unsigned long long total = stat.f_blocks * stat.f_frsize;
            unsigned long long free = stat.f_bfree * stat.f_frsize;
            unsigned long long available = stat.f_bavail * stat.f_frsize;
            unsigned long long used = total - free;
            double percent = (total > 0) ? (100.0 * used / total) : 0.0;

            disk["path"] = path;
            disk["total"] = total;
            disk["used"] = used;
            disk["available"] = available;
            disk["percent"] = percent;
        } else {
            disk["error"] = "Failed to query filesystem";
        }
        return disk;
    }

    static void get_net_io(unsigned long long& rx, unsigned long long& tx) {
        rx = 0;
        tx = 0;
        std::ifstream file("/proc/net/dev");
        std::string line;
        // Skip first 2 header lines
        std::getline(file, line);
        std::getline(file, line);

        while (std::getline(file, line)) {
            size_t colon = line.find(':');
            if (colon == std::string::npos) continue;
            std::string iface = line.substr(0, colon);
            // remove spaces
            iface.erase(0, iface.find_first_not_of(" \t"));
            if (iface == "lo") continue; // skip loopback

            std::istringstream ss(line.substr(colon + 1));
            unsigned long long r_bytes, r_packets, r_errs, r_drop, r_fifo, r_frame, r_compressed, r_multicast;
            unsigned long long t_bytes;
            if (ss >> r_bytes >> r_packets >> r_errs >> r_drop >> r_fifo >> r_frame >> r_compressed >> r_multicast
                   >> t_bytes) {
                rx += r_bytes;
                tx += t_bytes;
            }
        }
    }

    json get_network_rates() {
        json net;
        auto now = std::chrono::steady_clock::now();
        double elapsed_sec = std::chrono::duration<double>(now - prev_time_).count();
        if (elapsed_sec <= 0.0) elapsed_sec = 1.0;

        unsigned long long current_rx = 0, current_tx = 0;
        get_net_io(current_rx, current_tx);

        unsigned long long rx_diff = (current_rx >= prev_rx_bytes_) ? (current_rx - prev_rx_bytes_) : 0;
        unsigned long long tx_diff = (current_tx >= prev_tx_bytes_) ? (current_tx - prev_tx_bytes_) : 0;

        double rx_speed_kb = (rx_diff / 1024.0) / elapsed_sec;
        double tx_speed_kb = (tx_diff / 1024.0) / elapsed_sec;

        prev_rx_bytes_ = current_rx;
        prev_tx_bytes_ = current_tx;
        prev_time_ = now;

        net["rx_total_bytes"] = current_rx;
        net["tx_total_bytes"] = current_tx;
        net["rx_speed_kbps"] = rx_speed_kb;
        net["tx_speed_kbps"] = tx_speed_kb;
        return net;
    }

    static json get_temperatures() {
        json temps = json::array();

        // 1. Check /sys/class/thermal/thermal_zone*
        for (int i = 0; i < 30; ++i) {
            std::string dir = "/sys/class/thermal/thermal_zone" + std::to_string(i);
            std::ifstream temp_file(dir + "/temp");
            if (!temp_file.is_open()) continue;

            long temp_millideg = 0;
            temp_file >> temp_millideg;
            double celsius = temp_millideg / 1000.0;

            std::string type = "thermal_zone" + std::to_string(i);
            std::ifstream type_file(dir + "/type");
            if (type_file.is_open()) {
                std::getline(type_file, type);
            }

            json t;
            t["raw_label"] = type;
            t["label"] = format_sensor_friendly_name(type);
            t["temp_c"] = celsius;
            temps.push_back(t);
        }

        // 2. Check /sys/class/hwmon/hwmon* if thermal_zones was empty
        if (temps.empty()) {
            for (int h = 0; h < 10; ++h) {
                std::string hwmon_dir = "/sys/class/hwmon/hwmon" + std::to_string(h);
                std::string name = "hwmon" + std::to_string(h);
                std::ifstream name_file(hwmon_dir + "/name");
                if (name_file.is_open()) {
                    std::getline(name_file, name);
                }

                for (int t_idx = 1; t_idx <= 10; ++t_idx) {
                    std::string input_path = hwmon_dir + "/temp" + std::to_string(t_idx) + "_input";
                    std::ifstream tf(input_path);
                    if (!tf.is_open()) continue;

                    long mdeg = 0;
                    tf >> mdeg;
                    double celsius = mdeg / 1000.0;

                    std::string label = name + " temp" + std::to_string(t_idx);
                    std::ifstream label_file(hwmon_dir + "/temp" + std::to_string(t_idx) + "_label");
                    if (label_file.is_open()) {
                        std::getline(label_file, label);
                    }

                    json t;
                    t["raw_label"] = label;
                    t["label"] = format_sensor_friendly_name(label);
                    t["temp_c"] = celsius;
                    temps.push_back(t);
                }
            }
        }

        return temps;
    }

    static json get_system_info() {
        json info;
        char hostname[256] = {0};
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            info["hostname"] = std::string(hostname);
        } else {
            info["hostname"] = "unknown";
        }

        struct utsname u;
        if (uname(&u) == 0) {
            info["sysname"] = std::string(u.sysname);
            info["release"] = std::string(u.release);
            info["version"] = std::string(u.version);
            info["machine"] = std::string(u.machine);
        }

        // Read Uptime
        std::ifstream uptime_file("/proc/uptime");
        double uptime_sec = 0;
        if (uptime_file.is_open()) {
            uptime_file >> uptime_sec;
            info["uptime_seconds"] = static_cast<long long>(uptime_sec);
        }

        // Read OS Release pretty name
        std::ifstream os_file("/etc/os-release");
        std::string line;
        std::string os_pretty = "Linux (Debian-based)";
        while (std::getline(os_file, line)) {
            if (line.rfind("PRETTY_NAME=", 0) == 0) {
                os_pretty = line.substr(12);
                if (os_pretty.size() >= 2 && os_pretty.front() == '"' && os_pretty.back() == '"') {
                    os_pretty = os_pretty.substr(1, os_pretty.size() - 2);
                }
                break;
            }
        }
        info["os_name"] = os_pretty;

        return info;
    }

    json get_all_metrics() {
        json j;
        j["cpu_percent"] = get_cpu_usage();
        j["ram"] = get_ram_usage();
        j["disk"] = get_disk_usage("/");
        j["network"] = get_network_rates();
        j["temperatures"] = get_temperatures();
        j["system"] = get_system_info();
        return j;
    }
};
