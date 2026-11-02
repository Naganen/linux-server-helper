#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <unistd.h>
#include <sys/stat.h>

#include "httplib.h"
#include "json.hpp"
#include "config.hpp"
#include "metrics.hpp"
#include "process_manager.hpp"
#include "service_manager.hpp"
#include "user_manager.hpp"
#include "command_executor.hpp"
#include "power_manager.hpp"

using json = nlohmann::json;

static std::unique_ptr<httplib::Server> g_server = nullptr;

void signal_handler(int sig) {
    if (g_server) {
        std::cout << "\nStopping server due to signal " << sig << "...\n";
        g_server->stop();
    }
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS); // Parent exits

    if (setsid() < 0) exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    if (chdir("/") < 0) {
        // ignore
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char* argv[]) {
    std::string config_path = "/etc/linux-server-helper/config.json";
    bool run_as_daemon = false;

    // Check if running locally or config exists in current dir
    std::ifstream local_cfg("config.json");
    if (local_cfg.good()) {
        config_path = "config.json";
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-c" || arg == "--config") && i + 1 < argc) {
            config_path = argv[++i];
        } else if (arg == "-d" || arg == "--daemon") {
            run_as_daemon = true;
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Linux Server Helper Daemon (Debian)\n"
                      << "Usage: " << argv[0] << " [options]\n"
                      << "Options:\n"
                      << "  -c, --config <file>   Path to configuration JSON file\n"
                      << "  -d, --daemon          Run process in background as a daemon\n"
                      << "  -h, --help            Show this help message\n";
            return 0;
        }
    }

    ServerConfig config = ConfigManager::load_or_create(config_path);

    if (run_as_daemon) {
        daemonize();
    }

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    g_server = std::make_unique<httplib::Server>();
    MetricsCollector metrics;

    // Middleware: CORS & Pre-flight
    g_server->set_pre_routing_handler([](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization, X-API-Key");
        if (req.method == "OPTIONS") {
            res.status = 204;
            return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });

    // Authentication helper lambda
    auto is_authenticated = [&config](const httplib::Request& req) -> bool {
        if (config.api_token.empty()) return true;

        if (req.has_header("X-API-Key") && req.get_header_value("X-API-Key") == config.api_token) {
            return true;
        }

        if (req.has_header("Authorization")) {
            std::string auth = req.get_header_value("Authorization");
            if (auth.rfind("Bearer ", 0) == 0) {
                std::string token = auth.substr(7);
                if (token == config.api_token) return true;
            }
        }
        return false;
    };

    auto reject_unauthorized = [](httplib::Response& res) {
        json err;
        err["error"] = "Unauthorized. Provide valid Bearer token or X-API-Key";
        res.status = 401;
        res.set_content(err.dump(), "application/json");
    };

    // 1. Health check & Ping
    g_server->Get("/api/ping", [](const httplib::Request&, httplib::Response& res) {
        json j;
        j["status"] = "ok";
        j["name"] = "Linux Server Helper";
        j["version"] = "1.0.0";
        res.set_content(j.dump(), "application/json");
    });

    // 2. Metrics & System stats
    g_server->Get("/api/metrics", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        res.set_content(metrics.get_all_metrics().dump(), "application/json");
    });

    // 3. Process management
    g_server->Get("/api/processes", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        size_t limit = 150;
        if (req.has_param("limit")) {
            try { limit = std::stoul(req.get_param_value("limit")); } catch(...) {}
        }
        res.set_content(ProcessManager::get_process_list(limit).dump(), "application/json");
    });

    g_server->Post("/api/processes/kill", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        try {
            auto body = json::parse(req.body);
            int pid = body.value("pid", 0);
            int sig = body.value("signal", 15);
            bool ok = ProcessManager::kill_process(pid, sig);
            json r;
            r["success"] = ok;
            r["pid"] = pid;
            r["signal"] = sig;
            res.set_content(r.dump(), "application/json");
        } catch (const std::exception& e) {
            json r; r["error"] = e.what();
            res.status = 400;
            res.set_content(r.dump(), "application/json");
        }
    });

    // 4. Service management
    g_server->Get("/api/services", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        res.set_content(ServiceManager::list_services().dump(), "application/json");
    });

    g_server->Post("/api/services/action", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        try {
            auto body = json::parse(req.body);
            std::string service = body.value("service", "");
            std::string action = body.value("action", "");
            res.set_content(ServiceManager::service_action(service, action).dump(), "application/json");
        } catch (const std::exception& e) {
            json r; r["error"] = e.what();
            res.status = 400;
            res.set_content(r.dump(), "application/json");
        }
    });

    // 5. User management & Permissions
    g_server->Get("/api/users", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        res.set_content(UserManager::list_users().dump(), "application/json");
    });

    g_server->Post("/api/users/action", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        try {
            auto body = json::parse(req.body);
            std::string username = body.value("username", "");
            std::string action = body.value("action", "");
            std::string extra = body.value("extra", "");
            res.set_content(UserManager::modify_user_permission(username, action, extra).dump(), "application/json");
        } catch (const std::exception& e) {
            json r; r["error"] = e.what();
            res.status = 400;
            res.set_content(r.dump(), "application/json");
        }
    });

    // 6. Command Execution (Terminal)
    g_server->Post("/api/exec", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        try {
            auto body = json::parse(req.body);
            std::string cmd = body.value("command", "");
            res.set_content(CommandExecutor::execute(cmd).dump(), "application/json");
        } catch (const std::exception& e) {
            json r; r["error"] = e.what();
            res.status = 400;
            res.set_content(r.dump(), "application/json");
        }
    });

    // 7. Power Management (Reboot / Shutdown)
    g_server->Post("/api/power/reboot", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        int delay = 2;
        try {
            if (!req.body.empty()) {
                auto b = json::parse(req.body);
                delay = b.value("delay", 2);
            }
        } catch(...) {}
        res.set_content(PowerManager::schedule_reboot(delay).dump(), "application/json");
    });

    g_server->Post("/api/power/shutdown", [&](const httplib::Request& req, httplib::Response& res) {
        if (!is_authenticated(req)) return reject_unauthorized(res);
        int delay = 2;
        try {
            if (!req.body.empty()) {
                auto b = json::parse(req.body);
                delay = b.value("delay", 2);
            }
        } catch(...) {}
        res.set_content(PowerManager::schedule_shutdown(delay).dump(), "application/json");
    });

    std::cout << "====================================================\n";
    std::cout << "  Linux Server Helper Daemon started\n";
    std::cout << "  Listening on: http://" << config.host << ":" << config.port << "\n";
    std::cout << "  API Token   : " << config.api_token << "\n";
    std::cout << "====================================================\n";

    g_server->listen(config.host.c_str(), config.port);

    std::cout << "Server stopped.\n";
    return 0;
}

