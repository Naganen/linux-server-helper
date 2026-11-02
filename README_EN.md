# Linux Server Helper (Debian Daemon & Windows Modern Tray/Flyout Client)

This project allows you to monitor and manage **Debian Linux** servers via an invisible, ultra-lightweight C++ background daemon (`linux-server-helper`) paired with a modern Windows 11 Fluent Taskbar Flyout / Resizable Window desktop client.

Both the Linux daemon and Windows client are compiled with C++ (C++17), offering near-zero overhead (~3-5 MB RAM on Linux, ~340 KB executable size on Windows) and high native performance.

---

## 🚀 Key Features & Capabilities

1. **🌐 Multi-Server Management & Quick Switching:**
   - Register and manage multiple Linux servers (Name, IP, Port, Token).
   - Instant 1-click switching directly from the **Titlebar Dropdown Selector**.
   - Add, edit, or delete servers easily from the Settings tab.

2. **🚨 Critical Anomaly Monitoring & Dedicated Native Alert Window:**
   - **Background Scanning Across All Servers:** Monitors all saved servers in the background, not just the active one.
   - **Dedicated Win32 Alert Window:** Replaces disappearing notifications with a dedicated window anchored at the **bottom-right corner** of your screen that **requires user acknowledgment to close**.
   - **Aggregated Incident Reporting:** All simultaneous issues across servers (e.g. *Server 1: CPU 94%, Server 2: Temp 82 °C*) are presented in a single, consolidated alert.
   - **1-Minute Spam Protection:** Enforces a 60-second cooldown so your desktop is never flooded with alerts.
   - **Configurable Thresholds:** Easily customize warning limits for CPU, RAM, Disk fill rate, and Hardware temperatures.

3. **📊 Process Management with CPU %, RAM & Disk I/O:**
   - **Live CPU %:** Real-time processor usage calculated from `/proc/[pid]/stat` with dynamic load color indicators.
   - **RAM Usage (MB & %):** Exact memory footprint with percentage of total system RAM and a mini bar chart.
   - **Disk I/O (Read & Write):** Real-time cumulative disk read/write metrics parsed directly from `/proc/[pid]/io`.
   - **Sorting:** Sort processes instantly by **CPU %**, **RAM**, **Disk I/O**, **PID**, or **Name**.

4. **🌡️ Human-Friendly Hardware Temperatures:**
   - Translates raw sensor labels (`x86_pkg_temp`, `BAT0`, `acpitz`, `nvme0`) into clear names:
     - ⚡ **Processor (CPU)**
     - 🔋 **Battery**
     - 💾 **NVMe SSD Storage**
     - 🖥️ **Motherboard (ACPI)**
     - 📡 **Wireless Network (Wi-Fi)**
   - Custom SVG hardware icons and automatic health badges: **Optimal** (<45°C), **Normal** (45-70°C), **High** (>70°C).

5. **🌐 Full Dual-Language Support (English & Turkish):**
   - Click the **`TR | EN`** toggle in the top-right corner to translate the entire UI, table headers, and dialogues instantaneously.
   - Fully synced with native Windows context menus (*Open / Show*, *Switch to Window Mode*, *Exit*) and Alert window buttons.

6. **Systemd Service Management (Services):**
   - List and filter Debian services (`systemctl`) by status (All, Active Only, Failed).
   - Restart (`restart`), Stop (`stop`), or Start (`start`) any service with a single click.

7. **User & Permission Management (Users & Permissions):**
   - View system users, UID/GID, default shell, and group memberships.
   - One-click **Grant Sudo (Admin)** (`usermod -aG sudo`) or **Revoke Sudo** (`gpasswd -d ... sudo`).
   - Lock/unlock user accounts and modify default login shells.

8. **Remote Terminal & Power Control:**
   - Execute shell commands remotely (`df -h`, `docker ps`, `apt update`, etc.) with terminal output and execution duration.
   - Safely Reboot (`reboot`) or Shut Down (`shutdown`) the server remotely.

9. **Adaptive Windows UI Modes:**
   - **Flyout Mode (Default):** Opens smoothly above the taskbar in the bottom-right corner (`680 x 750 px`). Auto-hides on click outside.
   - **Window Mode:** Convert to a freely movable, resizable standard desktop window (`1050 x 780 px`) with title bar controls.

---

## 📁 Directory Structure

```
linux-server-helper/
├── linux-helper/                    # Debian Background Service (C++)
│   ├── src/
│   │   ├── main.cpp                 # REST API server & daemon loop
│   │   ├── metrics.hpp              # CPU, RAM, Disk, Network & Temp collector
│   │   ├── process_manager.hpp      # Process monitor (CPU%, RAM%, Disk I/O) & kill
│   │   ├── service_manager.hpp      # Systemd service controller
│   │   ├── user_manager.hpp         # User & sudo permission manager
│   │   ├── command_executor.hpp     # Shell command runner
│   │   ├── power_manager.hpp        # Reboot / Shutdown scheduler
│   │   └── config.hpp               # Config & token manager
│   ├── include/
│   │   ├── httplib.h                # Header-only HTTP server
│   │   └── json.hpp                 # nlohmann/json
│   ├── config.json.example          # Sample configuration
│   ├── linux-server-helper.service  # Systemd unit file
│   ├── Makefile                     # Build & install rules
│   └── install.sh                   # 1-Click Debian installer (automated UFW rule)
│
├── windows-client/                  # Windows Desktop Client (C++ Win32 & WebView2)
│   ├── bin/
│   │   ├── LinuxServerHelper.exe    # Compiled binary executable (~340 KB)
│   │   ├── WebView2Loader.dll
│   │   ├── resources/
│   │   └── ui/                      # Fluent Dark UI (HTML, CSS, JS, full i18n)
│   ├── src/
│   │   ├── main.cpp                 # Win32 Tray, Flyout & Alert Window (/utf-8)
│   │   └── ui/                      # Frontend sources
│   ├── packages/WebView2/           # Microsoft WebView2 SDK
│   └── build.bat                    # 1-Click build script (/utf-8)
│
├── test_mock_server.py              # Local testing & dev mock server
├── README.md                        # Bilingual Documentation (TR / EN)
└── README_EN.md                     # English Documentation
```

---

## 🐧 1. Debian Linux Server Setup (Helper)

Copy the `linux-helper` directory to your Debian machine and run:

```bash
cd linux-helper
sudo bash install.sh
```

### What the installer does:
1. Installs `build-essential` (g++, make) if missing.
2. Compiles the C++ source code with `-O3` optimizations.
3. Installs the binary to `/usr/local/bin/linux-server-helper`.
4. Generates a secure 32-character random **API Token** in `/etc/linux-server-helper/config.json`.
5. Creates and enables the systemd service (`linux-server-helper.service`) to run quietly on boot.
6. Automatically opens port `8765/tcp` if UFW firewall is active.
7. Prints your server's **Local IP**, **Port**, and **API Token** in a formatted banner.

### Service Commands:
```bash
# Check service status:
sudo systemctl status linux-server-helper

# View live service logs:
sudo journalctl -u linux-server-helper -f

# Restart service:
sudo systemctl restart linux-server-helper

# Stop service:
sudo systemctl stop linux-server-helper
```

---

## 🪟 2. Using the Windows Client

1. Launch `windows-client/bin/LinuxServerHelper.exe`.
2. The application sits quietly in your System Tray (bottom-right next to the clock).
3. Click the tray icon to open the control panel flyout.
4. Click the **`+`** button in the header or go to the **Settings** tab:
   - **Server Name**: e.g., `Production Debian`
   - **IP Address**: Server's IP address (e.g., `192.168.1.150`).
   - **Port**: `8765`
   - **API Token**: The secret token generated by `install.sh`.
   - Click **Save**.
5. When multiple servers are added, switch between them instantly using the top-left dropdown.

### 🌐 Switching Languages (EN / TR):
- Click the **`EN`** / **`TR`** pill button in the top-right header to toggle the entire application language immediately.

### 🪟 Switching to Window Mode:
- Click the maximize/window icon on the title bar or right-click the system tray icon and select *"Switch to Window Mode"*.

---

## 📡 3. REST API Reference (cURL & Automation)

Pass authentication via `X-API-Key: <TOKEN>` or `Authorization: Bearer <TOKEN>`.

```bash
# 1. Ping / Health check
curl http://SERVER_IP:8765/api/ping

# 2. Live Metrics (CPU, RAM, Disk, Network, Temps)
curl -H "X-API-Key: YOUR_TOKEN" http://SERVER_IP:8765/api/metrics

# 3. List Processes & Kill Process
curl -H "X-API-Key: YOUR_TOKEN" http://SERVER_IP:8765/api/processes?limit=150
curl -X POST -H "X-API-Key: YOUR_TOKEN" -H "Content-Type: application/json" -d '{"pid": 1234, "signal": 15}' http://SERVER_IP:8765/api/processes/kill

# 4. Service Actions (start, stop, restart, reload, status)
curl -H "X-API-Key: YOUR_TOKEN" http://SERVER_IP:8765/api/services
curl -X POST -H "X-API-Key: YOUR_TOKEN" -H "Content-Type: application/json" -d '{"service": "nginx.service", "action": "restart"}' http://SERVER_IP:8765/api/services/action

# 5. User Management & Permissions
curl -H "X-API-Key: YOUR_TOKEN" http://SERVER_IP:8765/api/users
curl -X POST -H "X-API-Key: YOUR_TOKEN" -H "Content-Type: application/json" -d '{"username": "john", "action": "grant_sudo"}' http://SERVER_IP:8765/api/users/action

# 6. Remote Shell Execution
curl -X POST -H "X-API-Key: YOUR_TOKEN" -H "Content-Type: application/json" -d '{"command": "uname -a && uptime"}' http://SERVER_IP:8765/api/exec

# 7. Power Management
curl -X POST -H "X-API-Key: YOUR_TOKEN" http://SERVER_IP:8765/api/power/reboot
curl -X POST -H "X-API-Key: YOUR_TOKEN" http://SERVER_IP:8765/api/power/shutdown
```

---

## 🛠️ Local Development & Mock Testing

To test the Windows client locally without a live Debian machine:

```bash
python test_mock_server.py
```

This starts a mock server at `http://127.0.0.1:8765` with token `TEST_TOKEN_12345`.

