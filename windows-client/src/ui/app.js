// Linux Server Helper - Client Frontend Logic (Multi-Server, Anomaly Monitoring & Full i18n)

const translations = {
    tr: {
        nav_overview: "Özet",
        nav_processes: "Süreçler",
        nav_services: "Servisler",
        nav_users: "Kullanıcılar",
        nav_terminal: "Terminal",
        nav_settings: "Ayarlar",

        searching_server: "Sunucu Aranıyor...",
        connected: "Bağlandı",
        not_connected: "Bağlı Değil",
        no_connection: "Bağlantı Yok",
        reboot_server: "Sunucuyu Yeniden Başlat",
        shutdown_server: "Sunucuyu Kapat",

        cpu_title: "İşlemci (CPU)",
        ram_title: "Bellek (RAM)",
        disk_title: "Disk Alanı (/)",
        net_title: "Ağ Trafiği",
        temps_title: "Sıcaklık Değerleri",
        download: "İndirme",
        upload: "Yükleme",
        total: "Toplam",
        sensors: "Sensör",
        scanning_sensors: "Sıcaklık sensörleri taranıyor...",
        no_sensors: "Sıcaklık sensörü bulunamadı (VM veya kısıtlı ortam).",
        ideal: "İdeal",
        normal: "Normal",
        high: "Yüksek",

        sensor_cpu: "İşlemci (CPU)",
        sensor_battery: "Batarya (Pil)",
        sensor_nvme: "NVMe SSD Depolama",
        sensor_acpi: "Anakart (ACPI)",
        sensor_wifi: "Kablosuz Ağ (Wi-Fi)",

        proc_search_ph: "Program, Kullanıcı veya PID ara...",
        sort_cpu: "İşlemci (CPU %) Göre",
        sort_ram: "RAM Kullanımına Göre",
        sort_disk: "Disk I/O Kullanımına Göre",
        sort_pid: "PID'ye Göre",
        sort_name: "Ada Göre",
        btn_refresh: "Yenile",
        loading_processes: "Süreçler yükleniyor...",
        no_processes: "Eşleşen süreç bulunamadı.",

        col_pid: "PID",
        col_name: "Program Adı",
        col_user: "Kullanıcı",
        col_cpu: "CPU %",
        col_ram: "RAM Kullanımı",
        col_disk_read: "Disk Okuma",
        col_disk_write: "Disk Yazma",
        col_state: "Durum",
        col_action: "İşlem",
        btn_kill: "Sonlandır",
        state_running: "Çalışıyor",
        state_sleeping: "Uyuyor",

        service_search_ph: "Servis adı ara (örn: ssh, nginx)...",
        all_services: "Tüm Servisler",
        only_active: "Yalnızca Aktif",
        failed_services: "Hatalı / Failed",
        col_service_unit: "Servis (Unit)",
        col_description: "Açıklama",
        col_actions: "Eylemler",
        loading_services: "Servisler taranıyor...",
        no_services: "Eşleşen servis bulunamadı.",
        btn_restart: "Restart",
        btn_stop: "Stop",
        btn_start: "Start",

        user_search_ph: "Kullanıcı ara...",
        col_username: "Kullanıcı",
        col_sudo_admin: "Sudo / Admin",
        col_shell: "Kabuk (Shell)",
        col_groups: "Gruplar",
        col_edit_perms: "Yetki Düzenle",
        loading_users: "Kullanıcılar yükleniyor...",
        no_users: "Kullanıcı bulunamadı.",
        btn_perms: "Yetkiler",
        badge_sudo: "SUDO / ADMIN",
        badge_standard: "Standart",

        modal_perm_title: "Yetki Düzenleme",
        home_dir: "Home Dizini",
        current_groups: "Mevcut Gruplar",
        sudo_root_perm: "Sudo / Root Yetkisi:",
        btn_revoke_sudo: "Sudo Yetkisini Kaldır",
        btn_grant_sudo: "Sudo (Yönetici) Yetkisi Ver",
        account_status: "Hesap Durumu:",
        btn_lock_account: "Hesabı Kilitle (Lock)",
        btn_unlock_account: "Kilidi Aç (Unlock)",
        change_shell: "Kabuk (Shell) Değiştir:",
        btn_apply: "Uygula",

        term_welcome: "Linux Server Helper - Uzak Komut Arayüzü",
        term_hint: "Örnek: 'uname -a', 'free -m', 'df -h', 'systemctl status ssh', 'docker ps'",
        term_input_ph: "Komut yazın ve Enter'a basın...",
        btn_send: "Gönder",

        saved_servers: "Kayıtlı Sunucular",
        btn_add_server: "+ Yeni Sunucu Ekle",
        badge_active: "Aktif",
        btn_connect: "Bağlan",
        btn_edit: "Düzenle",
        btn_delete: "Sil",

        server_info: "Sunucu Bilgileri",
        server_name_label: "Sunucu Adı (Etiket)",
        server_ip_label: "IP Adresi / Hostname",
        server_ip_hint: "Linux Helper servisinin çalıştığı sunucu adresi",
        server_token_label: "API Güvenlik Anahtarı (Token)",
        btn_show_token: "Göster",
        btn_hide_token: "Gizle",
        btn_save: "Kaydet",
        btn_cancel: "İptal",
        btn_test_conn: "Bağlantıyı Test Et",

        thresholds_title: "⚠️ Anormallik Uyarı Eşikleri & Arka Plan İzleme",
        thresholds_desc: "Kayıtlı sunuculardan herhangi biri bu eşikleri aşarsa özel bildirim penceresi açılır (Spam koruması: En fazla dakikada 1 bildirim).",
        thresh_cpu: "İşlemci (CPU) Alarm Eşiği (%)",
        thresh_ram: "Bellek (RAM) Alarm Eşiği (%)",
        thresh_disk: "Disk Doluluk Eşiği (%)",
        thresh_temp: "Sıcaklık Eşiği (°C)",
        refresh_rate_label: "Genel Metrik Yenileme Hızı",
        interval_1s: "1 Saniye (Çok Hızlı)",
        interval_2s: "2 Saniye (Önerilen)",
        interval_5s: "5 Saniye (Düşük Kaynak)",
        interval_10s: "10 Saniye",
        btn_save_thresholds: "Eşikleri ve Tercihleri Kaydet",

        alert_title: "⚠️ KRİTİK SUNUCU ANORMALLİK UYARISI",
        alert_heading: "Aşağıdaki sunucu anormallikleri tespit edildi:\n\n",
        alert_footer: "\n\nLütfen ilgili sunucunun durumunu ve servislerini kontrol edin.",
        issue_cpu: "İşlemci (CPU) Kritik Seviyede",
        issue_ram: "Bellek (RAM) Doluluğu Aşırı Yüksek",
        issue_disk: "Disk Alanı Kritik Dolulukta",
        issue_temp: "Sıcaklık Alarmı",
        threshold: "Eşik",

        uptime_day: "gün",
        uptime_hour: "sa",
        uptime_min: "dk"
    },
    en: {
        nav_overview: "Overview",
        nav_processes: "Processes",
        nav_services: "Services",
        nav_users: "Users",
        nav_terminal: "Terminal",
        nav_settings: "Settings",

        searching_server: "Searching for server...",
        connected: "Connected",
        not_connected: "Disconnected",
        no_connection: "No Connection",
        reboot_server: "Reboot Server",
        shutdown_server: "Shutdown Server",

        cpu_title: "Processor (CPU)",
        ram_title: "Memory (RAM)",
        disk_title: "Disk Space (/)",
        net_title: "Network Traffic",
        temps_title: "Temperature Readings",
        download: "Download",
        upload: "Upload",
        total: "Total",
        sensors: "Sensors",
        scanning_sensors: "Scanning temperature sensors...",
        no_sensors: "No temperature sensors found (VM or restricted environment).",
        ideal: "Optimal",
        normal: "Normal",
        high: "High",

        sensor_cpu: "Processor (CPU)",
        sensor_battery: "Battery",
        sensor_nvme: "NVMe SSD Storage",
        sensor_acpi: "Motherboard (ACPI)",
        sensor_wifi: "Wireless Network (Wi-Fi)",

        proc_search_ph: "Search process, user or PID...",
        sort_cpu: "By CPU %",
        sort_ram: "By RAM Usage",
        sort_disk: "By Disk I/O",
        sort_pid: "By PID",
        sort_name: "By Name",
        btn_refresh: "Refresh",
        loading_processes: "Loading processes...",
        no_processes: "No matching processes found.",

        col_pid: "PID",
        col_name: "Process Name",
        col_user: "User",
        col_cpu: "CPU %",
        col_ram: "RAM Usage",
        col_disk_read: "Disk Read",
        col_disk_write: "Disk Write",
        col_state: "State",
        col_action: "Action",
        btn_kill: "Kill",
        state_running: "Running",
        state_sleeping: "Sleeping",

        service_search_ph: "Search service (e.g. ssh, nginx)...",
        all_services: "All Services",
        only_active: "Active Only",
        failed_services: "Failed",
        col_service_unit: "Service (Unit)",
        col_description: "Description",
        col_actions: "Actions",
        loading_services: "Scanning services...",
        no_services: "No matching services found.",
        btn_restart: "Restart",
        btn_stop: "Stop",
        btn_start: "Start",

        user_search_ph: "Search user...",
        col_username: "User",
        col_sudo_admin: "Sudo / Admin",
        col_shell: "Shell",
        col_groups: "Groups",
        col_edit_perms: "Edit Perms",
        loading_users: "Loading users...",
        no_users: "No users found.",
        btn_perms: "Permissions",
        badge_sudo: "SUDO / ADMIN",
        badge_standard: "Standard",

        modal_perm_title: "Edit Permissions",
        home_dir: "Home Directory",
        current_groups: "Current Groups",
        sudo_root_perm: "Sudo / Root Permission:",
        btn_revoke_sudo: "Revoke Sudo Permission",
        btn_grant_sudo: "Grant Sudo (Admin) Permission",
        account_status: "Account Status:",
        btn_lock_account: "Lock Account",
        btn_unlock_account: "Unlock Account",
        change_shell: "Change Shell:",
        btn_apply: "Apply",

        term_welcome: "Linux Server Helper - Remote Shell Console",
        term_hint: "Examples: 'uname -a', 'free -m', 'df -h', 'systemctl status ssh', 'docker ps'",
        term_input_ph: "Type command and press Enter...",
        btn_send: "Send",

        saved_servers: "Saved Servers",
        btn_add_server: "+ Add New Server",
        badge_active: "Active",
        btn_connect: "Connect",
        btn_edit: "Edit",
        btn_delete: "Delete",

        server_info: "Server Details",
        server_name_label: "Server Name (Label)",
        server_ip_label: "IP Address / Hostname",
        server_ip_hint: "Address where Linux Helper service is running",
        server_token_label: "API Security Token",
        btn_show_token: "Show",
        btn_hide_token: "Hide",
        btn_save: "Save",
        btn_cancel: "Cancel",
        btn_test_conn: "Test Connection",

        thresholds_title: "⚠️ Anomaly Thresholds & Background Monitor",
        thresholds_desc: "A dedicated alert window opens if any registered server exceeds these limits (Spam protection: At most 1 alert per minute).",
        thresh_cpu: "CPU Alarm Threshold (%)",
        thresh_ram: "Memory (RAM) Alarm Threshold (%)",
        thresh_disk: "Disk Space Threshold (%)",
        thresh_temp: "Temperature Threshold (°C)",
        refresh_rate_label: "Metrics Refresh Interval",
        interval_1s: "1 Second (Very Fast)",
        interval_2s: "2 Seconds (Recommended)",
        interval_5s: "5 Seconds (Low Resource)",
        interval_10s: "10 Seconds",
        btn_save_thresholds: "Save Thresholds & Preferences",

        alert_title: "⚠️ CRITICAL SERVER ANOMALY ALERT",
        alert_heading: "The following server anomalies were detected:\n\n",
        alert_footer: "\n\nPlease check server health and background services.",
        issue_cpu: "CPU Usage at Critical Level",
        issue_ram: "Memory (RAM) Usage Extremely High",
        issue_disk: "Disk Space Critically Full",
        issue_temp: "Temperature Alarm",
        threshold: "Threshold",

        uptime_day: "d",
        uptime_hour: "h",
        uptime_min: "m"
    }
};

const state = {
    lang: localStorage.getItem('lsh_lang') || 'tr',
    servers: [],
    activeServerId: null,
    intervalMs: 2000,
    thresholds: {
        cpu: 85,
        ram: 90,
        disk: 90,
        temp: 80
    },
    lastAlertTime: 0,
    alertCooldownMs: 60000,
    isConnected: false,
    activeTab: 'tab-overview',
    refreshTimer: null,
    monitorTimer: null,
    processes: [],
    services: [],
    users: [],
    selectedUser: null
};

function t(key) {
    return (translations[state.lang] && translations[state.lang][key]) || (translations['tr'] && translations['tr'][key]) || key;
}

function applyLanguage(lang) {
    state.lang = lang;
    localStorage.setItem('lsh_lang', lang);
    postToNative('set_language', { lang });

    // Update Language Toggle Button
    const btnLang = document.getElementById('btn-lang-toggle');
    if (btnLang) {
        btnLang.textContent = (lang === 'tr') ? 'EN' : 'TR';
    }

    // Update all elements with data-i18n
    document.querySelectorAll('[data-i18n]').forEach(el => {
        const k = el.getAttribute('data-i18n');
        const translated = t(k);
        if (translated) {
            el.textContent = translated;
        }
    });

    // Update placeholders
    const procSearch = document.getElementById('proc-search');
    if (procSearch) procSearch.placeholder = t('proc_search_ph');

    const srvSearch = document.getElementById('service-search');
    if (srvSearch) srvSearch.placeholder = t('service_search_ph');

    const usrSearch = document.getElementById('user-search');
    if (usrSearch) usrSearch.placeholder = t('user_search_ph');

    const termInput = document.getElementById('term-input');
    if (termInput) termInput.placeholder = t('term_input_ph');

    // Update connection badge text if disconnected
    if (!state.isConnected) {
        updateConnectionStatus(false);
    }

    renderServersList();
    renderProcesses();
    renderServices();
    renderUsers();
}

// --- DOM Elements ---
const connDot = document.getElementById('connection-dot');
const hostBadge = document.getElementById('host-badge');
const serverSelect = document.getElementById('server-select');
const toastEl = document.getElementById('toast-msg');

function postToNative(action, data = {}) {
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage(JSON.stringify({ action, ...data }));
    } else {
        console.log('[Native Interop Emulation]', action, data);
    }
}

function showToast(msg, duration = 3000) {
    toastEl.textContent = msg;
    toastEl.classList.add('show');
    setTimeout(() => toastEl.classList.remove('show'), duration);
}

// --- Multi-Server Management ---
function loadServersFromStorage() {
    const raw = localStorage.getItem('lsh_servers');
    if (raw) {
        try {
            state.servers = JSON.parse(raw);
        } catch(e) {
            state.servers = [];
        }
    }

    if (!state.servers || state.servers.length === 0) {
        const legacyIp = localStorage.getItem('lsh_ip') || '127.0.0.1';
        const legacyPort = localStorage.getItem('lsh_port') || '8765';
        const legacyToken = localStorage.getItem('lsh_token') || '';
        state.servers = [{
            id: 'srv_' + Date.now(),
            name: (state.lang === 'en') ? 'Debian Main Server' : 'Debian Ana Sunucu',
            ip: legacyIp,
            port: legacyPort,
            token: legacyToken
        }];
        saveServersToStorage();
    }

    const activeId = localStorage.getItem('lsh_active_server_id');
    if (activeId && state.servers.some(s => s.id === activeId)) {
        state.activeServerId = activeId;
    } else {
        state.activeServerId = state.servers[0].id;
    }

    const rawThresh = localStorage.getItem('lsh_thresholds');
    if (rawThresh) {
        try { state.thresholds = { ...state.thresholds, ...JSON.parse(rawThresh) }; } catch(e) {}
    }

    state.intervalMs = parseInt(localStorage.getItem('lsh_interval') || '2000', 10);
}

function saveServersToStorage() {
    localStorage.setItem('lsh_servers', JSON.stringify(state.servers));
    localStorage.setItem('lsh_active_server_id', state.activeServerId);
}

function saveThresholdsToStorage() {
    localStorage.setItem('lsh_thresholds', JSON.stringify(state.thresholds));
    localStorage.setItem('lsh_interval', state.intervalMs.toString());
}

function getActiveServer() {
    return state.servers.find(s => s.id === state.activeServerId) || state.servers[0];
}

function switchServer(id) {
    if (state.activeServerId === id) return;
    const target = state.servers.find(s => s.id === id);
    if (!target) return;

    state.activeServerId = id;
    saveServersToStorage();
    renderServerDropdown();
    renderServersList();
    showToast(`${t('connected')}: ${target.name}`);

    restartMetricsLoop();
    if (state.activeTab === 'tab-processes') fetchProcesses();
    if (state.activeTab === 'tab-services') fetchServices();
    if (state.activeTab === 'tab-users') fetchUsers();
}

function renderServerDropdown() {
    if (!serverSelect) return;
    serverSelect.innerHTML = state.servers.map(s => `
        <option value="${s.id}" ${s.id === state.activeServerId ? 'selected' : ''}>
            ${s.name} (${s.ip})
        </option>
    `).join('');
}

function renderServersList() {
    const listContainer = document.getElementById('servers-list-container');
    if (!listContainer) return;

    listContainer.innerHTML = state.servers.map(s => {
        const isActive = s.id === state.activeServerId;
        return `
            <div class="server-item-row ${isActive ? 'active' : ''}">
                <div class="server-item-info">
                    <div style="display: flex; flex-direction: column; gap: 2px;">
                        <span class="server-item-name">${s.name}</span>
                        <span class="server-item-addr">${s.ip}:${s.port}</span>
                    </div>
                    ${isActive ? `<span class="server-active-pill">${t('badge_active')}</span>` : ''}
                </div>
                <div class="server-item-actions">
                    ${!isActive ? `<button class="btn-primary" onclick="window.switchServer('${s.id}')">${t('btn_connect')}</button>` : ''}
                    <button class="btn-secondary" onclick="window.editServerModal('${s.id}')">${t('btn_edit')}</button>
                    ${state.servers.length > 1 ? `<button class="btn-danger-sm" onclick="window.deleteServer('${s.id}')">${t('btn_delete')}</button>` : ''}
                </div>
            </div>
        `;
    }).join('');
}

window.switchServer = switchServer;

window.editServerModal = function(id) {
    const srv = state.servers.find(s => s.id === id);
    if (!srv) return;

    document.getElementById('cfg-server-id').value = srv.id;
    document.getElementById('cfg-name').value = srv.name;
    document.getElementById('cfg-ip').value = srv.ip;
    document.getElementById('cfg-port').value = srv.port;
    document.getElementById('cfg-token').value = srv.token;
    document.getElementById('form-server-title').textContent = `${t('btn_edit')}: ${srv.name}`;

    const card = document.getElementById('server-edit-card');
    card.style.display = 'flex';
    card.scrollIntoView({ behavior: 'smooth' });
};

window.deleteServer = function(id) {
    const srv = state.servers.find(s => s.id === id);
    if (!srv) return;
    const confirmMsg = (state.lang === 'en')
        ? `Are you sure you want to delete server '${srv.name}'?`
        : `'${srv.name}' sunucusunu listeden silmek istediğinize emin misiniz?`;

    if (confirm(confirmMsg)) {
        state.servers = state.servers.filter(s => s.id !== id);
        if (state.activeServerId === id) {
            state.activeServerId = state.servers[0].id;
        }
        saveServersToStorage();
        renderServerDropdown();
        renderServersList();
        restartMetricsLoop();
        showToast((state.lang === 'en') ? 'Server removed.' : 'Sunucu silindi.');
    }
};

// --- REST API Client ---
function getApiUrl(path, srv = null) {
    const target = srv || getActiveServer();
    return `http://${target.ip}:${target.port}${path}`;
}

async function apiRequest(path, options = {}, customServer = null) {
    const current = customServer || getActiveServer();
    const headers = {
        'Content-Type': 'application/json',
        ...(current.token ? { 'X-API-Key': current.token } : {})
    };

    const targetUrl = getApiUrl(path, current);

    try {
        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), 6000);

        const resp = await fetch(targetUrl, {
            ...options,
            signal: controller.signal,
            headers: { ...headers, ...options.headers }
        });
        clearTimeout(timeoutId);

        if (resp.status === 401) {
            if (!customServer || customServer.id === state.activeServerId) {
                updateConnectionStatus(false, (state.lang === 'en') ? 'Invalid Token' : 'Token Geçersiz');
            }
            throw new Error((state.lang === 'en') ? 'API Token is invalid or missing!' : 'API Token geçersiz veya eksik!');
        }

        if (!resp.ok) {
            throw new Error(`HTTP Error: ${resp.status}`);
        }

        const data = await resp.json();
        if (!customServer || customServer.id === state.activeServerId) {
            updateConnectionStatus(true);
        }
        return data;
    } catch (err) {
        let msg = err.message;
        if (err.name === 'AbortError') {
            msg = (state.lang === 'en') ? 'Timeout (Server unreachable)' : 'Zaman Aşımı (Sunucuya ulaşılamadı)';
        } else if (err.message === 'Failed to fetch' || err.name === 'TypeError') {
            msg = (state.lang === 'en') ? `Connection Failed (${current.ip}:${current.port})` : `Bağlantı Kurulamadı (${current.ip}:${current.port})`;
        }
        if (!customServer || customServer.id === state.activeServerId) {
            updateConnectionStatus(false, msg);
        }
        throw new Error(msg);
    }
}

function updateConnectionStatus(connected, message = '') {
    state.isConnected = connected;
    const current = getActiveServer();
    if (connected) {
        connDot.className = 'status-indicator online';
        hostBadge.textContent = `${current.ip}:${current.port}`;
        hostBadge.style.color = '#34d399';
    } else {
        connDot.className = 'status-indicator offline';
        hostBadge.textContent = message ? message : t('not_connected');
        hostBadge.style.color = '#f87171';
    }
}

function formatBytes(bytes) {
    if (!bytes || bytes === 0) return '0 B';
    const k = 1024;
    const sizes = ['B', 'KB', 'MB', 'GB', 'TB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(1)) + ' ' + sizes[i];
}

function formatUptime(seconds) {
    if (!seconds) return '-';
    const d = Math.floor(seconds / 86400);
    const h = Math.floor((seconds % 86400) / 3600);
    const m = Math.floor((seconds % 3600) / 60);
    let str = '';
    if (d > 0) str += `${d} ${t('uptime_day')} `;
    if (h > 0) str += `${h} ${t('uptime_hour')} `;
    str += `${m} ${t('uptime_min')}`;
    return str;
}

async function fetchMetrics() {
    try {
        const data = await apiRequest('/api/metrics');
        renderMetrics(data);
    } catch (err) {
        // Handled in updateConnectionStatus
    }
}

function renderMetrics(data) {
    const current = getActiveServer();
    if (data.system) {
        document.getElementById('sys-hostname').textContent = `${current.name} (${data.system.hostname || 'Linux'})`;
        document.getElementById('sys-os-kernel').textContent = 
            `${data.system.os_name || 'Debian'} (${data.system.release || ''}) | Uptime: ${formatUptime(data.system.uptime_seconds)}`;
    }

    // CPU
    const cpuUsage = Math.round(data.cpu_percent || 0);
    document.getElementById('cpu-val').textContent = `${cpuUsage}%`;
    document.getElementById('cpu-bar').style.width = `${Math.min(100, Math.max(0, cpuUsage))}%`;
    document.getElementById('cpu-sub').textContent = `%${cpuUsage}`;

    // RAM
    if (data.ram) {
        const ramPercent = Math.round(data.ram.percent || 0);
        document.getElementById('ram-val').textContent = `${ramPercent}%`;
        document.getElementById('ram-bar').style.width = `${ramPercent}%`;
        document.getElementById('ram-sub').textContent = `${formatBytes(data.ram.used)} / ${formatBytes(data.ram.total)}`;
    }

    // Disk
    if (data.disk) {
        const diskPercent = Math.round(data.disk.percent || 0);
        document.getElementById('disk-val').textContent = `${diskPercent}%`;
        document.getElementById('disk-bar').style.width = `${diskPercent}%`;
        document.getElementById('disk-sub').textContent = `${formatBytes(data.disk.used)} / ${formatBytes(data.disk.total)}`;
    }

    // Network
    if (data.network) {
        const rx = data.network.rx_speed_kbps || 0;
        const tx = data.network.tx_speed_kbps || 0;
        document.getElementById('net-speed').textContent = `${(rx + tx).toFixed(1)} KB/s`;
        document.getElementById('net-rx').textContent = `${rx.toFixed(1)} KB/s`;
        document.getElementById('net-tx').textContent = `${tx.toFixed(1)} KB/s`;
        document.getElementById('net-totals').textContent = 
            `${t('total')}: ${formatBytes(data.network.rx_total_bytes)} RX / ${formatBytes(data.network.tx_total_bytes)} TX`;
    }

    // Temperatures
    const tempContainer = document.getElementById('temp-chips');
    const temps = data.temperatures || [];
    document.getElementById('temp-count').textContent = `${temps.length} ${t('sensors')}`;

    if (temps.length === 0) {
        tempContainer.innerHTML = `<div class="empty-hint">${t('no_sensors')}</div>`;
    } else {
        tempContainer.innerHTML = temps.map(tItem => {
            const rawLabel = tItem.raw_label || tItem.label || 'Sensör';
            const tempVal = (tItem.temp_c !== undefined) ? tItem.temp_c.toFixed(1) : '-';
            const deg = parseFloat(tempVal) || 0;

            let statusClass = 'cool';
            let statusText = t('ideal');
            if (deg >= state.thresholds.temp) {
                statusClass = 'hot';
                statusText = t('high');
            } else if (deg >= 45) {
                statusClass = 'warm';
                statusText = t('normal');
            }

            const lower = rawLabel.toLowerCase();
            let friendlyName = tItem.label || rawLabel;
            let iconSvg = '<svg viewBox="0 0 24 24" width="20" height="20"><path fill="currentColor" d="M12 2a5 5 0 0 1 5 5v6a5 5 0 0 1-10 0V7a5 5 0 0 1 5-5z"/></svg>';

            if (lower.includes('x86_pkg') || lower.includes('pkg_temp') || lower.includes('cpu') || lower.includes('core')) {
                friendlyName = t('sensor_cpu');
                iconSvg = '<svg viewBox="0 0 24 24" width="20" height="20"><path fill="currentColor" d="M6 2h12a2 2 0 0 1 2 2v16a2 2 0 0 1-2 2H6a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2zm3 4H7v2h2V6zm8 0h-2v2h2V6zM9 10H7v2h2v-2zm8 0h-2v2h2v-2zM9 14H7v2h2v-2zm8 0h-2v2h2v-2zm-8 4H7v2h2v-2zm8 0h-2v2h2v-2z"/></svg>';
            } else if (lower.startsWith('bat') || lower.includes('battery')) {
                friendlyName = t('sensor_battery');
                iconSvg = '<svg viewBox="0 0 24 24" width="20" height="20"><path fill="currentColor" d="M16 4h-2V2h-4v2H8C6.9 4 6 4.9 6 6v14c0 1.1.9 2 2 2h8c1.1 0 2-.9 2-2V6c0-1.1-.9-2-2-2zm0 16H8V6h8v14z"/></svg>';
            } else if (lower.includes('nvme') || lower.includes('composite') || lower.includes('disk')) {
                friendlyName = t('sensor_nvme');
                iconSvg = '<svg viewBox="0 0 24 24" width="20" height="20"><path fill="currentColor" d="M6 2h12a2 2 0 0 1 2 2v16a2 2 0 0 1-2 2H6a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2zm6 15a2 2 0 1 0 0-4 2 2 0 0 0 0 4z"/></svg>';
            } else if (lower.includes('acpitz')) {
                friendlyName = t('sensor_acpi');
                iconSvg = '<svg viewBox="0 0 24 24" width="20" height="20"><path fill="currentColor" d="M4 4h16v16H4V4zm2 2v12h12V6H6zm3 2h2v2H9V8zm4 0h2v2h-2V8zm-4 4h2v2H9v-2zm4 0h2v2h-2v-2z"/></svg>';
            } else if (lower.includes('wifi') || lower.includes('iwlwifi') || lower.includes('wlan')) {
                friendlyName = t('sensor_wifi');
                iconSvg = '<svg viewBox="0 0 24 24" width="20" height="20"><path fill="currentColor" d="M12 4C7.31 4 3.07 5.9 0 8.98L12 21 24 8.98A16.88 16.88 0 0 0 12 4z"/></svg>';
            }

            return `
                <div class="temp-card" title="Sensör: ${rawLabel}">
                    <div class="temp-icon-box ${statusClass}">
                        ${iconSvg}
                    </div>
                    <div class="temp-details">
                        <div class="temp-title-row">
                            <span class="temp-friendly-name">${friendlyName}</span>
                            <span class="temp-raw-badge">${rawLabel}</span>
                        </div>
                        <div class="temp-value-row">
                            <span class="temp-degrees ${statusClass}">${tempVal} °C</span>
                            <span class="temp-status-text ${statusClass}">${statusText}</span>
                        </div>
                    </div>
                </div>
            `;
        }).join('');
    }
}

// -----------------------------------------------------------------
// BACKGROUND ALL-SERVERS ANOMALY MONITOR & SPAM-PROTECTED ALERTS
// -----------------------------------------------------------------
async function checkAnomaliesForAllServers() {
    const issues = [];
    const now = Date.now();

    for (const srv of state.servers) {
        try {
            const data = await apiRequest('/api/metrics', {}, srv);

            const cpu = data.cpu_percent || 0;
            if (cpu >= state.thresholds.cpu) {
                issues.push(`• [${srv.name}] ${t('issue_cpu')}: %${cpu.toFixed(1)} (${t('threshold')}: %${state.thresholds.cpu})`);
            }

            if (data.ram && data.ram.percent >= state.thresholds.ram) {
                issues.push(`• [${srv.name}] ${t('issue_ram')}: %${data.ram.percent.toFixed(1)} (${t('threshold')}: %${state.thresholds.ram})`);
            }

            if (data.disk && data.disk.percent >= state.thresholds.disk) {
                issues.push(`• [${srv.name}] ${t('issue_disk')}: %${data.disk.percent.toFixed(1)} (${t('threshold')}: %${state.thresholds.disk})`);
            }

            if (data.temperatures && Array.isArray(data.temperatures)) {
                for (const tItem of data.temperatures) {
                    if (tItem.temp_c && tItem.temp_c >= state.thresholds.temp) {
                        const lbl = tItem.label || tItem.raw_label || 'Sensor';
                        issues.push(`• [${srv.name}] ${t('issue_temp')} (${lbl}): ${tItem.temp_c.toFixed(1)} °C (${t('threshold')}: ${state.thresholds.temp} °C)`);
                    }
                }
            }
        } catch (err) {}
    }

    if (issues.length > 0) {
        if ((now - state.lastAlertTime) >= state.alertCooldownMs) {
            state.lastAlertTime = now;
            const fullMessage = t('alert_heading') + 
                                issues.join("\n") + 
                                t('alert_footer');

            postToNative('show_alert_window', {
                title: t('alert_title'),
                message: fullMessage
            });
        }
    }
}

// --- Processes Tab ---
async function fetchProcesses() {
    try {
        const list = await apiRequest('/api/processes?limit=150');
        state.processes = list;
        renderProcesses();
    } catch (err) {
        showToast(err.message);
    }
}

function renderProcesses() {
    const query = document.getElementById('proc-search').value.toLowerCase().trim();
    const sortBy = document.getElementById('proc-sort') ? document.getElementById('proc-sort').value : 'cpu';
    const tbody = document.getElementById('proc-tbody');

    let filtered = state.processes.filter(p => {
        return p.name.toLowerCase().includes(query) || 
               p.user.toLowerCase().includes(query) || 
               p.pid.toString().includes(query);
    });

    filtered.sort((a, b) => {
        if (sortBy === 'cpu') {
            return (b.cpu_percent || 0) - (a.cpu_percent || 0);
        } else if (sortBy === 'ram') {
            return (b.memory_kb || 0) - (a.memory_kb || 0);
        } else if (sortBy === 'disk') {
            const diskA = (a.disk_read_bytes || 0) + (a.disk_write_bytes || 0);
            const diskB = (b.disk_read_bytes || 0) + (b.disk_write_bytes || 0);
            return diskB - diskA;
        } else if (sortBy === 'pid') {
            return a.pid - b.pid;
        } else if (sortBy === 'name') {
            return a.name.localeCompare(b.name);
        }
        return 0;
    });

    if (filtered.length === 0) {
        tbody.innerHTML = `<tr><td colspan="9" class="text-center">${t('no_processes')}</td></tr>`;
        return;
    }

    tbody.innerHTML = filtered.map(p => {
        const cpuVal = (p.cpu_percent !== undefined) ? p.cpu_percent.toFixed(1) : '0.0';
        let cpuColor = '#94a3b8';
        if (parseFloat(cpuVal) > 50) cpuColor = '#f87171';
        else if (parseFloat(cpuVal) > 15) cpuColor = '#fbbf24';
        else if (parseFloat(cpuVal) > 1) cpuColor = '#38bdf8';

        const memMb = (p.memory_mb || 0).toFixed(1);
        const memPercent = (p.memory_percent !== undefined && p.memory_percent > 0) ? p.memory_percent.toFixed(1) : null;
        const diskRead = formatBytes(p.disk_read_bytes || 0);
        const diskWrite = formatBytes(p.disk_write_bytes || 0);
        const stateLabel = (p.state === 'R') ? t('state_running') : t('state_sleeping');

        return `
            <tr>
                <td><strong>${p.pid}</strong></td>
                <td><span style="font-weight: 600; color: #fff;">${p.name}</span></td>
                <td><span style="color: #94a3b8;">${p.user}</span></td>
                <td>
                    <span style="font-weight: 700; color: ${cpuColor}; font-family: var(--font-mono);">
                        %${cpuVal}
                    </span>
                </td>
                <td>
                    <div class="ram-cell-box">
                        <span>${memMb} MB ${memPercent ? `<span style="color:#64748b; font-size:10px;">(%${memPercent})</span>` : ''}</span>
                        ${memPercent ? `<div class="ram-bar-mini"><div class="ram-bar-mini-fill" style="width: ${Math.min(100, memPercent * 4)}%;"></div></div>` : ''}
                    </div>
                </td>
                <td><span class="disk-io-tag">↓ ${diskRead}</span></td>
                <td><span class="disk-io-tag">↑ ${diskWrite}</span></td>
                <td><span class="badge-status badge-${p.state === 'R' ? 'active' : 'inactive'}">${stateLabel}</span></td>
                <td>
                    <button class="btn-danger-sm" onclick="killProcessPrompt(${p.pid}, '${p.name}')">${t('btn_kill')}</button>
                </td>
            </tr>
        `;
    }).join('');
}

window.killProcessPrompt = function(pid, name) {
    const confirmMsg = (state.lang === 'en')
        ? `Are you sure you want to kill process '${name}' (PID: ${pid})?`
        : `'${name}' (PID: ${pid}) sürecini sonlandırmak istediğinize emin misiniz?`;

    if (confirm(confirmMsg)) {
        apiRequest('/api/processes/kill', {
            method: 'POST',
            body: JSON.stringify({ pid, signal: 15 })
        }).then(res => {
            showToast(`${name} (${pid}) ${t('btn_kill')} OK.`);
            fetchProcesses();
        }).catch(err => showToast(err.message));
    }
};

// --- Services Tab ---
async function fetchServices() {
    try {
        const list = await apiRequest('/api/services');
        state.services = list;
        renderServices();
    } catch (err) {
        showToast(err.message);
    }
}

function renderServices() {
    const query = document.getElementById('service-search').value.toLowerCase().trim();
    const filter = document.getElementById('service-filter').value;
    const tbody = document.getElementById('service-tbody');

    const filtered = state.services.filter(s => {
        const matchesQuery = s.unit.toLowerCase().includes(query) || (s.description && s.description.toLowerCase().includes(query));
        if (!matchesQuery) return false;
        if (filter === 'active') return s.active === 'active';
        if (filter === 'failed') return s.active === 'failed' || s.sub === 'failed';
        return true;
    });

    if (filtered.length === 0) {
        tbody.innerHTML = `<tr><td colspan="4" class="text-center">${t('no_services')}</td></tr>`;
        return;
    }

    tbody.innerHTML = filtered.map(s => {
        let badgeClass = 'badge-inactive';
        if (s.active === 'active') badgeClass = 'badge-active';
        if (s.active === 'failed') badgeClass = 'badge-failed';

        return `
            <tr>
                <td><strong>${s.unit}</strong></td>
                <td><span class="badge-status ${badgeClass}">${s.active} (${s.sub})</span></td>
                <td style="max-width: 250px; overflow: hidden; text-overflow: ellipsis; white-space: nowrap;">${s.description || '-'}</td>
                <td>
                    <button class="btn-action-sm" onclick="serviceAction('${s.unit}', 'restart')">${t('btn_restart')}</button>
                    ${s.active === 'active' 
                        ? `<button class="btn-action-sm" onclick="serviceAction('${s.unit}', 'stop')">${t('btn_stop')}</button>`
                        : `<button class="btn-action-sm" onclick="serviceAction('${s.unit}', 'start')">${t('btn_start')}</button>`
                    }
                </td>
            </tr>
        `;
    }).join('');
}

window.serviceAction = function(unit, action) {
    showToast(`${unit} -> ${action}...`);
    apiRequest('/api/services/action', {
        method: 'POST',
        body: JSON.stringify({ service: unit, action })
    }).then(res => {
        showToast(`${unit} ${action} OK!`);
        setTimeout(fetchServices, 1000);
    }).catch(err => showToast(err.message));
};

// --- Users & Permissions Tab ---
async function fetchUsers() {
    try {
        const list = await apiRequest('/api/users');
        state.users = list;
        renderUsers();
    } catch (err) {
        showToast(err.message);
    }
}

function renderUsers() {
    const query = document.getElementById('user-search').value.toLowerCase().trim();
    const tbody = document.getElementById('users-tbody');

    const filtered = state.users.filter(u => {
        return u.username.toLowerCase().includes(query) || (u.groups && u.groups.join(' ').toLowerCase().includes(query));
    });

    if (filtered.length === 0) {
        tbody.innerHTML = `<tr><td colspan="6" class="text-center">${t('no_users')}</td></tr>`;
        return;
    }

    tbody.innerHTML = filtered.map(u => `
        <tr>
            <td><strong>${u.username}</strong></td>
            <td>${u.uid}</td>
            <td>
                ${u.is_sudo 
                    ? `<span class="badge-status badge-sudo">${t('badge_sudo')}</span>` 
                    : `<span class="badge-status badge-inactive">${t('badge_standard')}</span>`
                }
            </td>
            <td><code>${u.shell}</code></td>
            <td style="max-width: 180px; overflow: hidden; text-overflow: ellipsis; white-space: nowrap;">
                ${(u.groups || []).join(', ')}
            </td>
            <td>
                <button class="btn-action-sm" onclick="openUserModal('${u.username}')">${t('btn_perms')}</button>
            </td>
        </tr>
    `).join('');
}

window.openUserModal = function(username) {
    const user = state.users.find(u => u.username === username);
    if (!user) return;
    state.selectedUser = user;

    document.getElementById('modal-title').textContent = `${t('modal_perm_title')}: ${user.username}`;
    document.getElementById('modal-body').innerHTML = `
        <div style="line-height: 1.6;">
            <div><strong>UID / GID:</strong> ${user.uid} / ${user.gid}</div>
            <div><strong>${t('home_dir')}:</strong> ${user.home}</div>
            <div><strong>${t('current_groups')}:</strong> ${(user.groups || []).join(', ')}</div>
        </div>
        <hr style="border: 0; border-top: 1px solid var(--border-color); margin: 6px 0;">
        <div class="form-group">
            <label>${t('sudo_root_perm')}</label>
            <div>
                ${user.is_sudo
                    ? `<button class="btn-danger-sm" onclick="userPermissionAction('revoke_sudo')">${t('btn_revoke_sudo')}</button>`
                    : `<button class="btn-primary" onclick="userPermissionAction('grant_sudo')">${t('btn_grant_sudo')}</button>`
                }
            </div>
        </div>
        <div class="form-group">
            <label>${t('account_status')}</label>
            <div>
                <button class="btn-secondary" onclick="userPermissionAction('lock')">${t('btn_lock_account')}</button>
                <button class="btn-secondary" onclick="userPermissionAction('unlock')">${t('btn_unlock_account')}</button>
            </div>
        </div>
        <div class="form-group">
            <label>${t('change_shell')}</label>
            <div style="display: flex; gap: 6px;">
                <input type="text" id="modal-shell-input" class="text-input" value="${user.shell}">
                <button class="btn-secondary" onclick="userPermissionAction('set_shell')">${t('btn_save')}</button>
            </div>
        </div>
    `;

    document.getElementById('modal-container').classList.add('open');
};

window.userPermissionAction = function(action) {
    if (!state.selectedUser) return;
    const username = state.selectedUser.username;
    let extra = '';

    if (action === 'set_shell') {
        extra = document.getElementById('modal-shell-input').value.trim();
    }

    apiRequest('/api/users/action', {
        method: 'POST',
        body: JSON.stringify({ username, action, extra })
    }).then(res => {
        showToast(`${username} -> ${action} OK`);
        document.getElementById('modal-container').classList.remove('open');
        fetchUsers();
    }).catch(err => showToast(err.message));
};

document.getElementById('modal-close-btn').addEventListener('click', () => {
    document.getElementById('modal-container').classList.remove('open');
});
document.getElementById('modal-cancel').addEventListener('click', () => {
    document.getElementById('modal-container').classList.remove('open');
});
document.getElementById('modal-confirm').addEventListener('click', () => {
    document.getElementById('modal-container').classList.remove('open');
});

// --- Terminal Console ---
const termScreen = document.getElementById('term-screen');
const termForm = document.getElementById('term-form');
const termInput = document.getElementById('term-input');

termForm.addEventListener('submit', async (e) => {
    e.preventDefault();
    const cmd = termInput.value.trim();
    if (!cmd) return;

    termInput.value = '';
    appendTerminalLine(`server:~$ ${cmd}`, 'cmd-in');

    try {
        const res = await apiRequest('/api/exec', {
            method: 'POST',
            body: JSON.stringify({ command: cmd })
        });

        if (res.output) {
            appendTerminalLine(res.output, res.exit_code === 0 ? 'cmd-out' : 'cmd-err');
        }
        appendTerminalLine(`[Exit code: ${res.exit_code} | ${res.duration_ms} ms]`, 'cmd-meta');
    } catch (err) {
        appendTerminalLine(`Error: ${err.message}`, 'cmd-err');
    }
});

function appendTerminalLine(text, className = '') {
    const div = document.createElement('div');
    div.className = `term-line ${className}`;
    div.textContent = text;
    termScreen.appendChild(div);
    termScreen.scrollTop = termScreen.scrollHeight;
}

// --- Quick Power Actions ---
document.getElementById('btn-quick-reboot').addEventListener('click', () => {
    const srv = getActiveServer();
    const confirmMsg = (state.lang === 'en')
        ? `Are you sure you want to REBOOT server '${srv.name}'?`
        : `'${srv.name}' sunucusunu YENİDEN BAŞLATMAK istediğinize emin misiniz?`;

    if (confirm(confirmMsg)) {
        apiRequest('/api/power/reboot', { method: 'POST' })
            .then(r => showToast((state.lang === 'en') ? 'Server rebooting...' : 'Sunucu yeniden başlatılıyor...'))
            .catch(err => showToast(err.message));
    }
});

document.getElementById('btn-quick-shutdown').addEventListener('click', () => {
    const srv = getActiveServer();
    const confirmMsg = (state.lang === 'en')
        ? `Are you sure you want to SHUT DOWN server '${srv.name}'?`
        : `'${srv.name}' sunucusunu TAMAMEN KAPATMAK istediğinize emin misiniz?`;

    if (confirm(confirmMsg)) {
        apiRequest('/api/power/shutdown', { method: 'POST' })
            .then(r => showToast((state.lang === 'en') ? 'Server shutting down...' : 'Sunucu kapatılıyor...'))
            .catch(err => showToast(err.message));
    }
});

// --- Settings & Server Form ---
const cfgServerId = document.getElementById('cfg-server-id');
const cfgName = document.getElementById('cfg-name');
const cfgIp = document.getElementById('cfg-ip');
const cfgPort = document.getElementById('cfg-port');
const cfgToken = document.getElementById('cfg-token');
const cfgInterval = document.getElementById('cfg-interval');
const btnToggleToken = document.getElementById('btn-toggle-token-view');
const connStatusMsg = document.getElementById('conn-status-msg');
const serverEditCard = document.getElementById('server-edit-card');

const cfgThresholdCpu = document.getElementById('cfg-threshold-cpu');
const cfgThresholdRam = document.getElementById('cfg-threshold-ram');
const cfgThresholdDisk = document.getElementById('cfg-threshold-disk');
const cfgThresholdTemp = document.getElementById('cfg-threshold-temp');

function populateThresholdInputs() {
    cfgThresholdCpu.value = state.thresholds.cpu;
    cfgThresholdRam.value = state.thresholds.ram;
    cfgThresholdDisk.value = state.thresholds.disk;
    cfgThresholdTemp.value = state.thresholds.temp;
    cfgInterval.value = state.intervalMs.toString();
}

document.getElementById('btn-show-add-server').addEventListener('click', () => {
    cfgServerId.value = '';
    cfgName.value = (state.lang === 'en') ? 'New Linux Server' : 'Yeni Linux Sunucu';
    cfgIp.value = '';
    cfgPort.value = '8765';
    cfgToken.value = '';
    document.getElementById('form-server-title').textContent = t('btn_add_server');
    serverEditCard.style.display = 'flex';
    serverEditCard.scrollIntoView({ behavior: 'smooth' });
});

document.getElementById('btn-quick-add-server').addEventListener('click', () => {
    document.querySelector('.tab-btn[data-tab="tab-settings"]').click();
    document.getElementById('btn-show-add-server').click();
});

document.getElementById('btn-cancel-server-edit').addEventListener('click', () => {
    serverEditCard.style.display = 'none';
});

btnToggleToken.addEventListener('click', () => {
    if (cfgToken.type === 'password') {
        cfgToken.type = 'text';
        btnToggleToken.textContent = t('btn_hide_token');
    } else {
        cfgToken.type = 'password';
        btnToggleToken.textContent = t('btn_show_token');
    }
});

document.getElementById('btn-save-server').addEventListener('click', () => {
    const id = cfgServerId.value.trim();
    const name = cfgName.value.trim() || 'Debian Server';
    const ip = cfgIp.value.trim() || '127.0.0.1';
    const port = cfgPort.value.trim() || '8765';
    const token = cfgToken.value.trim();

    if (id) {
        const srv = state.servers.find(s => s.id === id);
        if (srv) {
            srv.name = name;
            srv.ip = ip;
            srv.port = port;
            srv.token = token;
        }
    } else {
        const newId = 'srv_' + Date.now();
        state.servers.push({ id: newId, name, ip, port, token });
        state.activeServerId = newId;
    }

    saveServersToStorage();
    renderServerDropdown();
    renderServersList();
    serverEditCard.style.display = 'none';
    showToast(t('btn_save') + ' OK!');
    restartMetricsLoop();
});

document.getElementById('btn-save-thresholds').addEventListener('click', () => {
    state.thresholds.cpu = parseInt(cfgThresholdCpu.value, 10) || 85;
    state.thresholds.ram = parseInt(cfgThresholdRam.value, 10) || 90;
    state.thresholds.disk = parseInt(cfgThresholdDisk.value, 10) || 90;
    state.thresholds.temp = parseInt(cfgThresholdTemp.value, 10) || 80;
    state.intervalMs = parseInt(cfgInterval.value, 10) || 2000;

    saveThresholdsToStorage();
    showToast((state.lang === 'en') ? 'Settings updated!' : 'Ayarlar güncellendi!');
    restartMetricsLoop();
});

document.getElementById('btn-test-connection').addEventListener('click', testConnection);

async function testConnection() {
    connStatusMsg.className = 'connection-status-box';
    connStatusMsg.textContent = (state.lang === 'en') ? 'Testing connection...' : 'Bağlantı test ediliyor...';
    connStatusMsg.style.display = 'block';

    const testServer = {
        id: cfgServerId.value || 'test',
        name: cfgName.value || 'Test',
        ip: cfgIp.value.trim() || '127.0.0.1',
        port: cfgPort.value.trim() || '8765',
        token: cfgToken.value.trim()
    };

    try {
        const ping = await apiRequest('/api/ping', {}, testServer);
        connStatusMsg.className = 'connection-status-box success';
        connStatusMsg.textContent = (state.lang === 'en') 
            ? `Connection Successful! Server: ${ping.name} v${ping.version}`
            : `Bağlantı Başarılı! Sunucu: ${ping.name} v${ping.version}`;
    } catch (err) {
        connStatusMsg.className = 'connection-status-box error';
        connStatusMsg.textContent = `${err.message}`;
    }
}

// --- Tabs Management ---
document.querySelectorAll('.tab-btn').forEach(btn => {
    btn.addEventListener('click', () => {
        document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
        document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));

        btn.classList.add('active');
        const tabId = btn.getAttribute('data-tab');
        document.getElementById(tabId).classList.add('active');
        state.activeTab = tabId;

        if (tabId === 'tab-processes') fetchProcesses();
        if (tabId === 'tab-services') fetchServices();
        if (tabId === 'tab-users') fetchUsers();
        if (tabId === 'tab-settings') {
            renderServersList();
            populateThresholdInputs();
        }
    });
});

// Search & Sort filters
document.getElementById('proc-search').addEventListener('input', renderProcesses);
const procSortEl = document.getElementById('proc-sort');
if (procSortEl) procSortEl.addEventListener('change', renderProcesses);
document.getElementById('service-search').addEventListener('input', renderServices);
document.getElementById('service-filter').addEventListener('change', renderServices);
document.getElementById('user-search').addEventListener('input', renderUsers);

document.getElementById('btn-refresh-procs').addEventListener('click', fetchProcesses);
document.getElementById('btn-refresh-services').addEventListener('click', fetchServices);
document.getElementById('btn-refresh-users').addEventListener('click', fetchUsers);

serverSelect.addEventListener('change', (e) => {
    switchServer(e.target.value);
});

// --- Window Controls (Win32 Native Interop) ---
document.getElementById('btn-toggle-mode').addEventListener('click', () => {
    postToNative('toggle_window_mode');
});
document.getElementById('btn-minimize').addEventListener('click', () => {
    postToNative('minimize');
});
document.getElementById('btn-close').addEventListener('click', () => {
    postToNative('hide');
});

// Language Toggle
const btnLangToggle = document.getElementById('btn-lang-toggle');
if (btnLangToggle) {
    btnLangToggle.addEventListener('click', () => {
        const nextLang = (state.lang === 'tr') ? 'en' : 'tr';
        applyLanguage(nextLang);
        showToast(nextLang === 'en' ? 'Language switched to English' : 'Dil Türkçe olarak ayarlandı');
    });
}

// Periodic Loops
function restartMetricsLoop() {
    if (state.refreshTimer) clearInterval(state.refreshTimer);
    fetchMetrics();
    state.refreshTimer = setInterval(() => {
        if (state.activeTab === 'tab-overview') {
            fetchMetrics();
        }
    }, state.intervalMs);
}

function startBackgroundMonitor() {
    if (state.monitorTimer) clearInterval(state.monitorTimer);
    checkAnomaliesForAllServers();
    state.monitorTimer = setInterval(checkAnomaliesForAllServers, 12000);
}

// Initialize on page load
window.addEventListener('DOMContentLoaded', () => {
    loadServersFromStorage();
    applyLanguage(state.lang);
    renderServerDropdown();
    renderServersList();
    populateThresholdInputs();
    restartMetricsLoop();
    startBackgroundMonitor();
});
