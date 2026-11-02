#pragma once
#include <string>
#include <array>
#include <memory>
#include <chrono>
#include "json.hpp"

using json = nlohmann::json;

class CommandExecutor {
public:
    static json execute(const std::string& command) {
        json resp;
        if (command.empty()) {
            resp["exit_code"] = 0;
            resp["output"] = "";
            resp["duration_ms"] = 0;
            return resp;
        }

        auto start = std::chrono::steady_clock::now();

        // Redirect stderr to stdout to capture everything
        std::string full_cmd = "{ " + command + "; } 2>&1";

        std::array<char, 512> buffer;
        std::string output;
        FILE* pipe = popen(full_cmd.c_str(), "r");
        if (!pipe) {
            resp["exit_code"] = -1;
            resp["output"] = "Failed to spawn process";
            resp["duration_ms"] = 0;
            return resp;
        }

        // Limit maximum output to 512 KB to prevent memory exhaustion
        const size_t MAX_OUTPUT_BYTES = 512 * 1024;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            if (output.size() < MAX_OUTPUT_BYTES) {
                output += buffer.data();
            }
        }

        int status = pclose(pipe);
        int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

        auto end = std::chrono::steady_clock::now();
        long long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        resp["exit_code"] = exit_code;
        resp["output"] = output;
        resp["duration_ms"] = duration_ms;
        return resp;
    }
};

