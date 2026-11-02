#!/bin/bash
set -e

# Terminal colors
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m' # No Color

echo -e "${CYAN}${BOLD}"
echo "========================================================"
echo "    Linux Server Helper - Debian Automated Installer    "
echo "========================================================"
echo -e "${NC}"

if [ "$EUID" -ne 0 ]; then
  echo -e "${RED}[!] Lutfen bu kurulum scriptini root veya sudo ile calistirin:${NC}"
  echo "    sudo bash install.sh"
  exit 1
fi

echo -e "${YELLOW}[1/5] Gerekli paketler kontrol ediliyor (g++, make, build-essential)...${NC}"
if ! command -v g++ >/dev/null 2>&1 || ! command -v make >/dev/null 2>&1; then
    echo -e "${CYAN}    Paketler yukleniyor: apt update && apt install -y build-essential${NC}"
    apt-get update -y
    apt-get install -y build-essential
fi

echo -e "${YELLOW}[2/5] C++ Kaynak kodlari derleniyor (O3 optimizasyonu ile)...${NC}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

make clean || true
make -j$(nproc)

echo -e "${YELLOW}[3/5] Binary ve yapilandirma dosyalari yukleniyor...${NC}"
install -d /usr/local/bin
install -m 755 linux-server-helper /usr/local/bin/linux-server-helper
install -d /etc/linux-server-helper

CONFIG_FILE="/etc/linux-server-helper/config.json"
if [ ! -f "$CONFIG_FILE" ]; then
    # Generate secure random 32 character hex token
    API_TOKEN=$(head /dev/urandom | tr -dc A-Za-z0-9 | head -c 32)
    cat <<EOF > "$CONFIG_FILE"
{
    "host": "0.0.0.0",
    "port": 8765,
    "api_token": "$API_TOKEN",
    "allow_remote": true
}
EOF
    chmod 600 "$CONFIG_FILE"
    echo -e "${GREEN}    Yeni guvenlik anahtari (Token) olusturuldu.${NC}"
else
    echo -e "${CYAN}    Mevcut yapilandirma dosyasi korundu ($CONFIG_FILE).${NC}"
    API_TOKEN=$(grep -o '"api_token": "[^"]*' "$CONFIG_FILE" | cut -d'"' -f4)
fi

echo -e "${YELLOW}[4/5] Systemd arka plan servisi kuruluyor...${NC}"
cp -f linux-server-helper.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable linux-server-helper.service
systemctl restart linux-server-helper.service

# Check & configure firewall (UFW / iptables)
if command -v ufw >/dev/null 2>&1; then
    if ufw status | grep -q "Status: active"; then
        echo -e "${CYAN}    UFW guvenlik duvari tespit edildi, 8765/tcp portu aciliyor...${NC}"
        ufw allow 8765/tcp >/dev/null 2>&1 || true
    fi
fi

echo -e "${YELLOW}[5/5] Servis durumu kontrol ediliyor...${NC}"
sleep 1
if systemctl is-active --quiet linux-server-helper; then
    echo -e "${GREEN}${BOLD}[OK] Servis basariyla baslatildi ve arka planda calisiyor!${NC}"
else
    echo -e "${RED}[!] Servis baslatilamadi. Loglari inceleyin: journalctl -u linux-server-helper -e${NC}"
fi

# Detect Local IP Addresses
LOCAL_IPS=$(hostname -I | tr ' ' '\n' | grep -v '^$' | grep -v '^127\.' || true)
PRIMARY_IP=$(echo "$LOCAL_IPS" | head -n 1)
PORT=8765

echo ""
echo -e "${GREEN}================================================================${NC}"
echo -e "${GREEN}${BOLD}             KURULUM BASARIYLA TAMAMLANDI!                      ${NC}"
echo -e "${GREEN}================================================================${NC}"
echo -e "${BOLD}Windows Programinda Girilecek Bilgiler:${NC}"
echo -e "  Server IP    : ${CYAN}${BOLD}${PRIMARY_IP:-"SERVER_IP_ADRESINIZ"}${NC}"
echo -e "  Port         : ${CYAN}${BOLD}${PORT}${NC}"
echo -e "  API Token    : ${YELLOW}${BOLD}${API_TOKEN}${NC}"
echo ""
echo -e "${BOLD}Servis Komutlari:${NC}"
echo -e "  Durum        : systemctl status linux-server-helper"
echo -e "  Durdur       : systemctl stop linux-server-helper"
echo -e "  Baslat       : systemctl start linux-server-helper"
echo -e "  Yeniden Baslat: systemctl restart linux-server-helper"
echo -e "  Canli Loglar : journalctl -u linux-server-helper -f"
echo -e "${GREEN}================================================================${NC}"

