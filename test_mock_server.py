#!/usr/bin/env python3
"""
Linux Server Helper - Test & Development Mock Server
Bu script, yerel Windows veya Linux makinenizde Debian Linux Helper API'sini simüle eder.
Windows istemcisini anında test etmek için kullanılabilir.
"""

from http.server import HTTPServer, BaseHTTPRequestHandler
import json
import time
import random
import os
import platform

PORT = 8765
API_TOKEN = "TEST_TOKEN_12345"

class MockHelperHandler(BaseHTTPRequestHandler):
    def _set_headers(self, status=200):
        self.send_response(status)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type, Authorization, X-API-Key')
        self.end_headers()

    def do_OPTIONS(self):
        self._set_headers(204)

    def is_auth(self):
        token = self.headers.get('X-API-Key')
        auth = self.headers.get('Authorization', '')
        if auth.startswith('Bearer '):
            token = auth[7:]
        return token == API_TOKEN

    def do_GET(self):
        if self.path == '/api/ping':
            self._set_headers(200)
            self.wfile.write(json.dumps({
                "status": "ok",
                "name": "Linux Server Helper (Debian Mock)",
                "version": "1.0.0"
            }).encode('utf-8'))
            return

        if not self.is_auth():
            self._set_headers(401)
            self.wfile.write(json.dumps({"error": "Unauthorized"}).encode('utf-8'))
            return

        if self.path.startswith('/api/metrics'):
            self._set_headers(200)
            cpu = random.randint(15, 65)
            ram_used = 4200000000 + random.randint(10000000, 200000000)
            ram_total = 16777216000
            data = {
                "cpu_percent": cpu,
                "ram": {
                    "total": ram_total,
                    "used": ram_used,
                    "available": ram_total - ram_used,
                    "free": 8000000000,
                    "percent": round((ram_used / ram_total) * 100, 1)
                },
                "disk": {
                    "path": "/",
                    "total": 512000000000,
                    "used": 145000000000,
                    "available": 367000000000,
                    "percent": 28.3
                },
                "network": {
                    "rx_speed_kbps": round(random.uniform(50.0, 1500.0), 1),
                    "tx_speed_kbps": round(random.uniform(20.0, 450.0), 1),
                    "rx_total_bytes": 10542389100,
                    "tx_total_bytes": 4293810230
                },
                "temperatures": [
                    {"label": "İşlemci (CPU)", "raw_label": "x86_pkg_temp", "temp_c": round(42.0 + random.uniform(0.5, 4.0), 1)},
                    {"label": "Batarya (Pil)", "raw_label": "BAT0", "temp_c": round(28.5 + random.uniform(0.1, 1.0), 1)},
                    {"label": "Anakart (ACPI)", "raw_label": "acpitz", "temp_c": 36.5},
                    {"label": "NVMe SSD Depolama", "raw_label": "nvme0", "temp_c": 38.0}
                ],
                "system": {
                    "hostname": "debian-production-srv",
                    "sysname": "Linux",
                    "release": "6.1.0-21-amd64",
                    "version": "#1 SMP PREEMPT_DYNAMIC Debian 6.1.90-1",
                    "machine": "x86_64",
                    "uptime_seconds": 384200,
                    "os_name": "Debian GNU/Linux 12 (bookworm)"
                }
            }
            self.wfile.write(json.dumps(data).encode('utf-8'))
            return

        if self.path.startswith('/api/processes'):
            self._set_headers(200)
            procs = [
                {"pid": 1, "name": "systemd", "user": "root", "state": "S", "memory_mb": 14.2, "memory_percent": 0.1, "cpu_ticks": 120, "disk_read_bytes": 10485760, "disk_write_bytes": 5242880},
                {"pid": 412, "name": "systemd-journald", "user": "root", "state": "S", "memory_mb": 42.5, "memory_percent": 0.3, "cpu_ticks": 450, "disk_read_bytes": 45000000, "disk_write_bytes": 120000000},
                {"pid": 782, "name": "sshd", "user": "root", "state": "S", "memory_mb": 8.1, "memory_percent": 0.05, "cpu_ticks": 90, "disk_read_bytes": 1024000, "disk_write_bytes": 512000},
                {"pid": 920, "name": "dockerd", "user": "root", "state": "S", "memory_mb": 115.4, "memory_percent": 0.7, "cpu_ticks": 2840, "disk_read_bytes": 350000000, "disk_write_bytes": 820000000},
                {"pid": 1140, "name": "nginx", "user": "www-data", "state": "S", "memory_mb": 35.8, "memory_percent": 0.2, "cpu_ticks": 890, "disk_read_bytes": 85000000, "disk_write_bytes": 15000000},
                {"pid": 1520, "name": "postgres", "user": "postgres", "state": "S", "memory_mb": 240.6, "memory_percent": 1.5, "cpu_ticks": 4120, "disk_read_bytes": 1250000000, "disk_write_bytes": 2400000000},
                {"pid": 1845, "name": "linux-server-helper", "user": "root", "state": "S", "memory_mb": 3.8, "memory_percent": 0.02, "cpu_ticks": 45, "disk_read_bytes": 512000, "disk_write_bytes": 128000},
                {"pid": 2040, "name": "node", "user": "deploy", "state": "S", "memory_mb": 185.2, "memory_percent": 1.1, "cpu_ticks": 5120, "disk_read_bytes": 620000000, "disk_write_bytes": 410000000},
                {"pid": 2311, "name": "redis-server", "user": "redis", "state": "S", "memory_mb": 28.4, "memory_percent": 0.2, "cpu_ticks": 310, "disk_read_bytes": 45000000, "disk_write_bytes": 75000000}
            ]
            self.wfile.write(json.dumps(procs).encode('utf-8'))
            return

        if self.path.startswith('/api/services'):
            self._set_headers(200)
            services = [
                {"unit": "ssh.service", "load": "loaded", "active": "active", "sub": "running", "description": "OpenBSD Secure Shell server"},
                {"unit": "nginx.service", "load": "loaded", "active": "active", "sub": "running", "description": "A high performance web server and reverse proxy server"},
                {"unit": "docker.service", "load": "loaded", "active": "active", "sub": "running", "description": "Docker Application Container Engine"},
                {"unit": "postgresql.service", "load": "loaded", "active": "active", "sub": "running", "description": "PostgreSQL RDBMS"},
                {"unit": "ufw.service", "load": "loaded", "active": "active", "sub": "exited", "description": "Uncomplicated firewall"},
                {"unit": "cron.service", "load": "loaded", "active": "active", "sub": "running", "description": "Regular background program processing daemon"},
                {"unit": "linux-server-helper.service", "load": "loaded", "active": "active", "sub": "running", "description": "Linux Server Helper Background Service"},
                {"unit": "apache2.service", "load": "loaded", "active": "inactive", "sub": "dead", "description": "The Apache HTTP Server"}
            ]
            self.wfile.write(json.dumps(services).encode('utf-8'))
            return

        if self.path.startswith('/api/users'):
            self._set_headers(200)
            users = [
                {"username": "root", "uid": 0, "gid": 0, "home": "/root", "shell": "/bin/bash", "is_sudo": True, "groups": ["root"]},
                {"username": "debian", "uid": 1000, "gid": 1000, "home": "/home/debian", "shell": "/bin/bash", "is_sudo": True, "groups": ["debian", "sudo", "docker"]},
                {"username": "deploy", "uid": 1001, "gid": 1001, "home": "/home/deploy", "shell": "/bin/bash", "is_sudo": False, "groups": ["deploy", "docker"]},
                {"username": "www-data", "uid": 33, "gid": 33, "home": "/var/www", "shell": "/usr/sbin/nologin", "is_sudo": False, "groups": ["www-data"]},
                {"username": "postgres", "uid": 105, "gid": 111, "home": "/var/lib/postgresql", "shell": "/bin/bash", "is_sudo": False, "groups": ["postgres"]}
            ]
            self.wfile.write(json.dumps(users).encode('utf-8'))
            return

        self._set_headers(404)
        self.wfile.write(json.dumps({"error": "Not Found"}).encode('utf-8'))

    def do_POST(self):
        if not self.is_auth():
            self._set_headers(401)
            self.wfile.write(json.dumps({"error": "Unauthorized"}).encode('utf-8'))
            return

        content_length = int(self.headers.get('Content-Length', 0))
        body = {}
        if content_length > 0:
            try:
                body = json.loads(self.rfile.read(content_length).decode('utf-8'))
            except:
                pass

        if self.path == '/api/exec':
            cmd = body.get('command', '')
            self._set_headers(200)
            output = f"[Simulated Output for: {cmd}]\nLinux debian-production-srv 6.1.0-21-amd64 x86_64\nCommand completed successfully."
            self.wfile.write(json.dumps({"exit_code": 0, "output": output, "duration_ms": 12}).encode('utf-8'))
            return

        if self.path == '/api/services/action':
            svc = body.get('service', '')
            action = body.get('action', '')
            self._set_headers(200)
            self.wfile.write(json.dumps({"success": True, "service": svc, "action": action, "output": f"Service {svc} {action} succeeded."}).encode('utf-8'))
            return

        if self.path == '/api/processes/kill':
            pid = body.get('pid', 0)
            self._set_headers(200)
            self.wfile.write(json.dumps({"success": True, "pid": pid, "signal": 15}).encode('utf-8'))
            return

        if self.path == '/api/users/action':
            u = body.get('username', '')
            act = body.get('action', '')
            self._set_headers(200)
            self.wfile.write(json.dumps({"success": True, "username": u, "action": act, "output": f"User {u} permission {act} applied."}).encode('utf-8'))
            return

        if self.path == '/api/power/reboot' or self.path == '/api/power/shutdown':
            action = "reboot" if "reboot" in self.path else "shutdown"
            self._set_headers(200)
            self.wfile.write(json.dumps({"success": True, "action": action, "message": f"Server {action} executed."}).encode('utf-8'))
            return

        self._set_headers(404)
        self.wfile.write(json.dumps({"error": "Not Found"}).encode('utf-8'))

def run():
    server = HTTPServer(('127.0.0.1', PORT), MockHelperHandler)
    print("=" * 60)
    print(f"  Linux Server Helper - Test Mock Server Calisiyor")
    print(f"  Adres: http://127.0.0.1:{PORT}")
    print(f"  Token: {API_TOKEN}")
    print("=" * 60)
    print("Durdurmak icin Ctrl+C tuslarina basin...")
    server.serve_forever()

if __name__ == '__main__':
    run()

