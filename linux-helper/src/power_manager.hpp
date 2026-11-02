#pragma once
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "json.hpp"

using json = nlohmann::json;

class PowerManager {
public:
    static json schedule_reboot(int delay_seconds = 2) {
        json resp;
        resp["success"] = true;
        resp["action"] = "reboot";
        resp["delay_seconds"] = delay_seconds;
        resp["message"] = "System reboot scheduled in " + std::to_string(delay_seconds) + " seconds";

        std::thread([delay_seconds]() {
            std::this_thread::sleep_for(std::chrono::seconds(delay_seconds));
            int res = system("systemctl reboot || shutdown -r now");
            (void)res;
        }).detach();

        return resp;
    }

    static json schedule_shutdown(int delay_seconds = 2) {
        json resp;
        resp["success"] = true;
        resp["action"] = "shutdown";
        resp["delay_seconds"] = delay_seconds;
        resp["message"] = "System shutdown scheduled in " + std::to_string(delay_seconds) + " seconds";

        std::thread([delay_seconds]() {
            std::this_thread::sleep_for(std::chrono::seconds(delay_seconds));
            int res = system("systemctl poweroff || shutdown -h now");
            (void)res;
        }).detach();

        return resp;
    }
};

