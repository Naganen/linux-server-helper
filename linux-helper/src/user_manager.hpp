#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <array>
#include <fstream>
#include <algorithm>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include "json.hpp"

using json = nlohmann::json;

class UserManager {
private:
    static std::string exec_command(const std::string& cmd) {
        std::array<char, 256> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) return "";
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    static bool is_valid_name(const std::string& name) {
        if (name.empty() || name.size() > 32) return false;
        for (char c : name) {
            if (!std::isalnum(c) && c != '_' && c != '-') return false;
        }
        return true;
    }

public:
    static json list_users() {
        json users = json::array();
        std::ifstream file("/etc/passwd");
        if (!file.is_open()) return users;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream ss(line);
            std::string username, pass, uid_str, gid_str, comment, home, shell;
            if (std::getline(ss, username, ':') &&
                std::getline(ss, pass, ':') &&
                std::getline(ss, uid_str, ':') &&
                std::getline(ss, gid_str, ':') &&
                std::getline(ss, comment, ':') &&
                std::getline(ss, home, ':') &&
                std::getline(ss, shell, ':')) {

                int uid = std::stoi(uid_str);
                int gid = std::stoi(gid_str);

                json user_item;
                user_item["username"] = username;
                user_item["uid"] = uid;
                user_item["gid"] = gid;
                user_item["home"] = home;
                user_item["shell"] = shell;
                user_item["is_system"] = (uid < 1000 && uid != 0);

                // Find user groups
                json groups = json::array();
                bool is_sudo = false;

                struct passwd* pw = getpwnam(username.c_str());
                if (pw) {
                    int ngroups = 64;
                    gid_t groups_arr[64];
                    if (getgrouplist(username.c_str(), pw->pw_gid, groups_arr, &ngroups) != -1) {
                        for (int i = 0; i < ngroups; ++i) {
                            struct group* grp = getgrgid(groups_arr[i]);
                            if (grp) {
                                std::string gname(grp->gr_name);
                                groups.push_back(gname);
                                if (gname == "sudo" || gname == "wheel" || (username == "root")) {
                                    is_sudo = true;
                                }
                            }
                        }
                    }
                }
                user_item["groups"] = groups;
                user_item["is_sudo"] = is_sudo;

                users.push_back(user_item);
            }
        }
        return users;
    }

    static json modify_user_permission(const std::string& username, const std::string& action, const std::string& extra_param = "") {
        json resp;
        if (!is_valid_name(username)) {
            resp["success"] = false;
            resp["error"] = "Invalid username";
            return resp;
        }

        std::string cmd;
        if (action == "grant_sudo") {
            cmd = "usermod -aG sudo " + username + " 2>&1";
        } else if (action == "revoke_sudo") {
            // Check if gpasswd or deluser exists
            cmd = "gpasswd -d " + username + " sudo 2>&1 || deluser " + username + " sudo 2>&1";
        } else if (action == "set_shell") {
            if (extra_param.empty() || extra_param.find(' ') != std::string::npos) {
                resp["success"] = false;
                resp["error"] = "Invalid shell path";
                return resp;
            }
            cmd = "usermod -s " + extra_param + " " + username + " 2>&1";
        } else if (action == "lock") {
            cmd = "usermod -L " + username + " 2>&1";
        } else if (action == "unlock") {
            cmd = "usermod -U " + username + " 2>&1";
        } else if (action == "add_group") {
            if (!is_valid_name(extra_param)) {
                resp["success"] = false;
                resp["error"] = "Invalid group name";
                return resp;
            }
            cmd = "usermod -aG " + extra_param + " " + username + " 2>&1";
        } else if (action == "remove_group") {
            if (!is_valid_name(extra_param)) {
                resp["success"] = false;
                resp["error"] = "Invalid group name";
                return resp;
            }
            cmd = "gpasswd -d " + username + " " + extra_param + " 2>&1";
        } else {
            resp["success"] = false;
            resp["error"] = "Unsupported action: " + action;
            return resp;
        }

        std::string output = exec_command(cmd);
        resp["success"] = true;
        resp["action"] = action;
        resp["username"] = username;
        resp["output"] = output;
        return resp;
    }
};

