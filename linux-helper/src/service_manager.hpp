#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <array>
#include <algorithm>
#include "json.hpp"

using json = nlohmann::json;

struct ServiceUnit {
    std::string unit;
    std::string load;
    std::string active;
    std::string sub;
    std::string description;
};

class ServiceManager {
private:
    static std::string exec_command(const std::string& cmd) {
        std::array<char, 256> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            return "";
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    static bool is_valid_service_name(const std::string& name) {
        if (name.empty() || name.size() > 100) return false;
        for (char c : name) {
            if (!std::isalnum(c) && c != '.' && c != '-' && c != '_' && c != '@') {
                return false;
            }
        }
        return true;
    }

public:
    static json list_services() {
        json services = json::array();
        std::string output = exec_command("systemctl list-units --type=service --all --no-pager --plain --no-legend 2>/dev/null");
        std::istringstream stream(output);
        std::string line;

        while (std::getline(stream, line)) {
            if (line.empty()) continue;
            std::istringstream line_stream(line);
            std::string unit, load, active, sub;
            if (line_stream >> unit >> load >> active >> sub) {
                std::string desc;
                std::getline(line_stream, desc);
                // Trim leading spaces
                size_t first = desc.find_first_not_of(" \t");
                if (first != std::string::npos) {
                    desc = desc.substr(first);
                }

                json item;
                item["unit"] = unit;
                item["load"] = load;
                item["active"] = active;
                item["sub"] = sub;
                item["description"] = desc;
                services.push_back(item);
            }
        }
        return services;
    }

    static json service_action(const std::string& service, const std::string& action) {
        json resp;
        if (!is_valid_service_name(service)) {
            resp["success"] = false;
            resp["error"] = "Invalid service name";
            return resp;
        }

        std::string act = action;
        std::transform(act.begin(), act.end(), act.begin(), ::tolower);
        if (act != "start" && act != "stop" && act != "restart" && act != "status" &&
            act != "enable" && act != "disable" && act != "reload") {
            resp["success"] = false;
            resp["error"] = "Invalid action. Supported: start, stop, restart, status, enable, disable, reload";
            return resp;
        }

        std::string cmd = "systemctl " + act + " " + service + " 2>&1";
        std::string output = exec_command(cmd);

        resp["success"] = true;
        resp["service"] = service;
        resp["action"] = act;
        resp["output"] = output;
        return resp;
    }
};

