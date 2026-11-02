#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include "json.hpp"

using json = nlohmann::json;

struct ServerConfig {
    std::string host = "0.0.0.0";
    int port = 8765;
    std::string api_token = "";
    bool allow_remote = true;
};

class ConfigManager {
public:
    static std::string generate_random_token(size_t length = 32) {
        const std::string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(0, (int)chars.size() - 1);

        std::string token = "";
        for (size_t i = 0; i < length; ++i) {
            token += chars[distribution(generator)];
        }
        return token;
    }

    static ServerConfig load_or_create(const std::string& path) {
        ServerConfig cfg;
        std::ifstream file(path);
        if (file.is_open()) {
            try {
                json j;
                file >> j;
                if (j.contains("host")) cfg.host = j["host"].get<std::string>();
                if (j.contains("port")) cfg.port = j["port"].get<int>();
                if (j.contains("api_token")) cfg.api_token = j["api_token"].get<std::string>();
                if (j.contains("allow_remote")) cfg.allow_remote = j["allow_remote"].get<bool>();
                file.close();
                return cfg;
            } catch (...) {
                // Ignore parse errors, fallback to recreate
            }
            file.close();
        }

        // Generate default config
        cfg.api_token = generate_random_token();
        save(path, cfg);
        return cfg;
    }

    static bool save(const std::string& path, const ServerConfig& cfg) {
        json j;
        j["host"] = cfg.host;
        j["port"] = cfg.port;
        j["api_token"] = cfg.api_token;
        j["allow_remote"] = cfg.allow_remote;

        std::ofstream file(path);
        if (file.is_open()) {
            file << j.dump(4);
            file.close();
            return true;
        }
        return false;
    }
};

