# Linux Server Helper (Debian Daemon & Windows Modern Tray/Flyout Client)

<p align="center">
  <b><a href="#-türkçe">Türkçe</a></b> | <b><a href="#-english">English</a></b>
</p>

---

## 📸 Ekran Görüntüleri / Screenshots

<div align="center">

| 1. 📊 Özet & Canlı Metrikler (Overview) | 2. ⚡ Süreçler & CPU / Disk (Processes) |
| :---: | :---: |
| <img src="https://github.com/user-attachments/assets/ecf4f16d-31db-4f23-8209-bca6925158f1" width="400" alt="Overview Tab" /> | <img src="https://github.com/user-attachments/assets/1ad5669c-e574-4f93-ab20-84dc2725f466" width="400" alt="Processes Tab" /> |

| 3. ⚙️ Servis Yönetimi (Services) | 4. 👥 Kullanıcılar & Yetkiler (Users) |
| :---: | :---: |
| <img src="https://github.com/user-attachments/assets/5db633bf-1cff-4e0b-b4f9-d9a85329cc7d" width="400" alt="Services Tab" /> | <img src="https://github.com/user-attachments/assets/52beb918-3e1e-406e-8dce-078780d5d833" width="400" alt="Users Tab" /> |

| 5. 💻 Uzak Terminal Konsolu (Terminal) | 6. 🌐 Ayarlar & Çoklu Sunucu (Settings) |
| :---: | :---: |
| <img src="https://github.com/user-attachments/assets/0410b013-5cac-4447-ba8c-3eef7b466167" width="400" alt="Terminal Tab" /> | <img src="https://github.com/user-attachments/assets/63c2b602-52d0-4ae8-872a-f9af22731490" width="400" alt="Settings Tab" /> |

### 7. 🚨 Sağ Alt Köşe Özel Anormallik Uyarı Penceresi (Dedicated Alert Window)
<img src="https://github.com/user-attachments/assets/63e6f54b-9332-4626-84a7-47883302c4f9" width="480" alt="Dedicated Alert Window" />

</div>

---

# 🇹🇷 Türkçe

Bu proje, **Debian Linux** sunucularınızı arka planda çalışan görünmez, ultra hafif bir C++ servisi (`linux-server-helper`) üzerinden izlemenizi ve **Windows** cihazınızdaki sistem tepsisinde (System Tray) yer alan modern Fluent flyout/pencere kontrol paneli ile yönetmenizi sağlar.

Hem Linux servisi hem de Windows istemcisi C++ (C++17) ile derlenmiştir, aşırı hafiftir (~3-5 MB RAM Linux, ~340 KB Windows ikili boyutu) ve yerel performans sunar.

---

## 🚀 Temel Yetenekler ve Özellikler

1. **🌐 Çoklu Sunucu Yönetimi & Hızlı Geçiş:**
   - Birden fazla Linux sunucusunu (Ad, IP, Port, Token) kaydetme.
   - Başlık çubuğundaki (Titlebar) **Sunucu Seçim Açılır Menüsü (Dropdown)** ile tek tıkla sunucular arasında anında geçiş yapma.
   - Ayarlar sekmesinde sunucu ekleme, düzenleme ve silme.

2. **🚨 Kritik Anormallik İzleme & Özel Uyarı Penceresi (Dedicated Alert Window):**
   - **Tüm Sunucuları Arka Planda Tarama:** Yalnızca aktif sunucuyu değil, kayıtlı tüm sunucuları arka planda periyodik olarak kontrol eder.
   - **Özel Win32 Bildirim Penceresi:** Standart kaybolan bildirimler yerine, **ekranın sağ alt köşesinde** açılan ve **kullanıcı onay vermeden kapanmayan** özel bir uyarı penceresi.
   - **Tek Seferde Toplu Raporlama:** Tespit edilen tüm anormallikler (örn: *Sunucu 1'de CPU %94, Sunucu 2'de Sıcaklık 82 °C*) tek bir raporda toplanarak iletilir.
   - **1 Dakika Spam Koruması:** Ekranda spam oluşmaması için en fazla 60 saniyede bir bildirim tetiklenir.
   - **Ayarlanabilir Eşikler:** Ayarlar sekmesinden CPU, RAM, Disk doluluğu ve Sıcaklık alarm eşiklerini dilediğiniz gibi belirleme.

3. **📊 Süreçler Tablosunda CPU %, RAM ve Disk I/O:**
   - **Canlı İşlemci (CPU %):** Her sürecin anlık CPU kullanım yüzdesi ve dinamik renk kodlaması.
   - **RAM (MB & %):** Sürecin harcadığı bellek MB ve toplam sisteme oranı (%) ile mini grafik çubuğu.
   - **Disk Kullanımı (I/O):** Linux `/proc/[pid]/io` sayacından sürecin diske yazdığı (Write) ve diskten okuduğu (Read) toplam bayt miktarları.
   - **Gelişmiş Sıralama:** Süreçleri **CPU %**, **RAM**, **Disk I/O**, **PID** veya **Ada** göre sıralama.

4. **🌡️ Anlamlı Donanım Sıcaklıkları:**
   - Ham sensör kodları (`x86_pkg_temp`, `BAT0`, `acpitz`, `nvme0`) yerine:
     - ⚡ **İşlemci (CPU)**
     - 🔋 **Batarya (Pil)**
     - 💾 **NVMe SSD Depolama**
     - 🖥️ **Anakart (ACPI)**
     - 📡 **Kablosuz Ağ (Wi-Fi)**
   - Özel SVG simgeleri ve otomatik durum rozetleri: **İdeal** (<45°C), **Normal** (45-70°C), **Yüksek** (>70°C).

5. **🌐 Çift Dil Desteği (Türkçe & English):**
   - Sağ üstteki **`TR | EN`** butonuyla tüm arayüzü tek tıkla İngilizce veya Türkçe'ye çevirme.
   - Windows sistem tepsisi menüsü (*Open / Show*, *Switch to Window Mode*, *Exit*) ve Uyarı penceresi dahil tam entegrasyon.

6. **Systemd Servis Yönetimi (Services):**
   - Debian servislerini (`systemctl`) anlık listeleme ve durum filtreleme (Tümü, Aktif, Hatalı).
   - Servisleri Yeniden Başlatma (`restart`), Durdurma (`stop`), Başlatma (`start`).

7. **Kullanıcılar ve Yetki Yönetimi (Users & Permissions):**
   - Sistem kullanıcılarını, UID/GID, kabuklarını ve gruplarını görüntüleme.
   - Tek tıkla kullanıcıya **Sudo (Yönetici) Yetkisi Verme** (`usermod -aG sudo`) veya **Kaldırma** (`gpasswd -d ... sudo`).
   - Kullanıcı hesabını kilitleme (`lock`) veya kilidi açma (`unlock`).
   - Kullanıcının varsayılan kabuğunu (shell) değiştirme.

8. **Uzak Terminal & Güç Yönetimi:**
   - Linux sunucusuna anında komut gönderme (`df -h`, `docker ps`, `apt update` vb.) ve renkli terminal çıktısı.
   - Sunucuyu uzaktan güvenle Yeniden Başlatma (`reboot`) ve Kapatma (`shutdown`).

9. **Genişletilmiş Windows Arayüz Modları:**
   - **Flyout Modu (Varsayılan):** Görev çubuğu simgesine basıldığında sağ altta (`680 x 750 px`) açılan şık kontrol paneli.
   - **Pencere Modu:** Tek tıkla serbestçe taşınabilen ve boyutlandırılabilen bağımsız pencereye (`1050 x 780 px`) dönüşebilme.

---

## 📁 Proje Dizin Yapısı

```
linux-server-helper/
├── linux-helper/                    # Debian Arka Plan Servisi (C++)
│   ├── src/
│   │   ├── main.cpp                 # REST API sunucusu, daemon döngüsü
│   │   ├── metrics.hpp              # CPU, RAM, Disk, Ağ ve Sıcaklık toplayıcı
│   │   ├── process_manager.hpp      # Süreç listeleme (CPU%, RAM%, Disk I/O) ve kill
│   │   ├── service_manager.hpp      # Systemd servis kontrolü
│   │   ├── user_manager.hpp         # Kullanıcı ve yetki (sudo/shell) yönetimi
│   │   ├── command_executor.hpp     # Shell komut çalıştırıcı
│   │   ├── power_manager.hpp        # Reboot / Shutdown
│   │   └── config.hpp               # Yapılandırma ve Token yöneticisi
│   ├── include/
│   │   ├── httplib.h                # Header-only HTTP sunucu
│   │   └── json.hpp                 # nlohmann/json
│   ├── config.json.example          # Örnek konfigürasyon
│   ├── linux-server-helper.service  # Systemd unit dosyası
│   ├── Makefile                     # Derleme ve yükleme talimatları
│   └── install.sh                   # Tek komutla otomatik Debian kurulumu (UFW port açma dahil)
│
├── windows-client/                  # Windows Masaüstü İstemcisi (C++ Win32 & WebView2)
│   ├── bin/
│   │   ├── LinuxServerHelper.exe    # Derlenmiş hazır çalıştırılabilir dosya (~340 KB)
│   │   ├── WebView2Loader.dll
│   │   ├── resources/
│   │   └── ui/                      # Fluent Dark Arayüz (HTML, CSS, JS, tam i18n)
│   ├── src/
│   │   ├── main.cpp                 # Win32 Tray, Flyout, Dedicated Alert Window (/utf-8)
│   │   └── ui/                      # Arayüz kaynak kodları
│   ├── packages/WebView2/           # Microsoft WebView2 SDK
│   └── build.bat                    # Tek tıkla Windows C++ derleme scripti (/utf-8)
│
├── test_mock_server.py              # Yerel test & geliştirme simülatörü
└── README.md
```

---

## 🐧 1. Debian Linux Sunucusuna Kurulum (Helper)

Debian sunucunuza `linux-helper` klasörünü kopyalayın ve tek bir komut çalıştırın:

```bash
cd linux-helper
sudo bash install.sh
```

### Kurulum Scriptinin Yaptıkları:
1. `build-essential` (g++, make) kurulu değilse otomatik yükler.
2. C++ kaynak kodlarını `-O3` optimizasyonuyla derler.
3. Binary'yi `/usr/local/bin/linux-server-helper` konumuna yerleştirir.
4. `/etc/linux-server-helper/config.json` dosyasına rastgele 32 karakterlik güçlü bir **API Token** üretir.
5. Systemd servisini (`linux-server-helper.service`) kurup `systemctl enable --now` ile arka planda sessizce başlatır.
6. Eğer Debian'da `ufw` güvenlik duvarı aktifse `8765/tcp` portunu otomatik açar.
7. Kurulum bittiğinde ekrana Windows programına girmeniz gereken **IP Adresi**, **Port** ve **API Token**'ı renkli kutucuk içinde yazdırır.

### Servis Yönetim Komutları:
```bash
# Servis durumunu kontrol etme:
sudo systemctl status linux-server-helper

# Canlı logları izleme:
sudo journalctl -u linux-server-helper -f

# Servisi yeniden başlatma:
sudo systemctl restart linux-server-helper

# Servisi durdurma:
sudo systemctl stop linux-server-helper
```

---

## 🪟 2. Windows İstemcisini Kullanma

1. `windows-client/bin/LinuxServerHelper.exe` dosyasını çalıştırın.
2. Program görev çubuğunda (sağ altta, saat yanında) bir sunucu simgesi olarak yerleşir.
3. Simgeye tıkladığınızda kontrol paneli açılır.
4. Sol üstteki **`+`** butonuna veya **Ayarlar (Settings)** sekmesine gidin:
   - **Sunucu Adı**: Örn. `Debian Ana Sunucu`
   - **IP Adresi**: Debian sunucunuzun yerel IP'si (örn. `192.168.1.150`).
   - **Port**: `8765`
   - **API Token**: `install.sh` çıktısında size verilen gizli anahtar.
   - **Kaydet** butonuna basın.
5. Birden fazla sunucu eklediğinizde, başlık çubuğundaki açılır listeden dilediğiniz sunucuya anında geçiş yapabilirsiniz.

### 🌐 Dil Değiştirme (TR / EN):
- Pencerenin sağ üstündeki **`TR | EN`** butonuna basarak tüm arayüzü, tablo sütunlarını ve bildirimleri saniyeler içinde İngilizce veya Türkçe yapabilirsiniz.

### 🪟 Pencere Moduna Geçiş:
- Sağ üstteki pencere simgesine basarak veya görev çubuğu simgesine sağ tıklayıp *"Pencere Moduna Geç"* diyerek istediğiniz zaman tam boyutlandırılabilir bağımsız pencereye dönüştürebilirsiniz.

---

## 📡 3. REST API Referansı (Harici İstekler ve Scripting)

Kendi scriptlerinizden veya cURL'den sunucunuza doğrudan istek atabilirsiniz. `X-API-Key: <TOKEN>` başlığı gereklidir.

```bash
# 1. Ping / Health
curl http://SUNUCU_IP:8765/api/ping

# 2. Canlı Metrikler (CPU, RAM, Disk, Ağ, Sıcaklık)
curl -H "X-API-Key: TOKEN" http://SUNUCU_IP:8765/api/metrics

# 3. Süreçleri Listeleme & Kill
curl -H "X-API-Key: TOKEN" http://SUNUCU_IP:8765/api/processes?limit=150
curl -X POST -H "X-API-Key: TOKEN" -H "Content-Type: application/json" -d '{"pid": 1234, "signal": 15}' http://SUNUCU_IP:8765/api/processes/kill

# 4. Servis Yönetimi
curl -H "X-API-Key: TOKEN" http://SUNUCU_IP:8765/api/services
curl -X POST -H "X-API-Key: TOKEN" -H "Content-Type: application/json" -d '{"service": "nginx.service", "action": "restart"}' http://SUNUCU_IP:8765/api/services/action

# 5. Kullanıcı Yönetimi & Sudo
curl -H "X-API-Key: TOKEN" http://SUNUCU_IP:8765/api/users
curl -X POST -H "X-API-Key: TOKEN" -H "Content-Type: application/json" -d '{"username": "ahmet", "action": "grant_sudo"}' http://SUNUCU_IP:8765/api/users/action

# 6. Uzak Komut Konsolu
curl -X POST -H "X-API-Key: TOKEN" -H "Content-Type: application/json" -d '{"command": "uname -a && uptime"}' http://SUNUCU_IP:8765/api/exec

# 7. Güç Yönetimi
curl -X POST -H "X-API-Key: TOKEN" http://SUNUCU_IP:8765/api/power/reboot
curl -X POST -H "X-API-Key: TOKEN" http://SUNUCU_IP:8765/api/power/shutdown
```

---
---

# 🇬🇧 English

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
