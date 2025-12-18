/*
 * ============================================================================
 * BLUETOOTH SPP (Serial Port Profile) IMPLEMENTIERUNG
 * ============================================================================
 * 
 * Beschreibung:
 * Dieses Modul implementiert Bluetooth Classic Communication über das
 * Serial Port Profile (SPP). Es ermöglicht serielle Datenübertragung
 * zwischen ESP32 und einem Client-Gerät (z.B. Smartphone, PC).
 * 
 * Funktionsweise:
 * - ESP32 agiert als SPP-Server (wartet auf Verbindungen)
 * - Client kann sich verbinden und Daten empfangen
 * - Daten werden als ASCII-Strings übertragen
 * 
 * Verwendete Profile:
 * - GAP (Generic Access Profile): Verbindungsverwaltung, Sichtbarkeit
 * - SPP (Serial Port Profile): Serielle Datenübertragung
 * 
 * Quellenhinweis:
 * Diese Implementierung basiert auf dem offiziellen ESP-IDF Beispielcode:
 * - ESP-IDF Bluetooth SPP Acceptor Example
 * - Quelle: https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/bluedroid/classic_bt/bt_spp_acceptor
 * - Angepasst und erweitert für das Waage-Projekt
 * 
 * ============================================================================
 */

#include "Bluetooth.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"

/////////////////////////////////////////////////////////////////////////////////////////
// KONFIGURATIONSKONSTANTEN
/////////////////////////////////////////////////////////////////////////////////////////

// Name des SPP-Servers (erscheint bei Verbindung)
// Wird verwendet wenn Client sich mit SPP verbindet
#define SPP_SERVER_NAME "ESP32_SPP_SERVER"

// Bluetooth-Gerätename (erscheint in Bluetooth-Geräteliste)
// Unter diesem Namen ist der ESP32 für andere Geräte sichtbar
#define DEVICE_NAME "ESP32_Waage"

// Tag für ESP_LOG Ausgaben (erscheint als [SPP_TAG] in Console)
#define SPP_TAG "SPP_ACCEPTOR_DEMO"

/////////////////////////////////////////////////////////////////////////////////////////
// GLOBALE VARIABLEN
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * Connection Handle
 * 
 * Speichert die Verbindungs-ID wenn ein Client verbunden ist.
 * - 0 = Keine Verbindung aktiv
 * - >0 = Verbindung aktiv, ID für Datenübertragung
 * 
 * Wird verwendet um:
 * - Zu prüfen ob ein Client verbunden ist
 * - Daten an den richtigen Client zu senden
 */
static uint32_t connection_handle = 0;

/////////////////////////////////////////////////////////////////////////////////////////
// BLUETOOTH GAP CALLBACK
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * GAP Event Callback Funktion
 * 
 * Diese Funktion wird vom Bluetooth-Stack aufgerufen wenn GAP-Events auftreten.
 * GAP (Generic Access Profile) verwaltet:
 * - Geräte-Sichtbarkeit (Discoverable/Connectable)
 * - Verbindungsaufbau
 * - Pairing/Authentifizierung
 * 
 * @param event Typ des aufgetretenen Events
 * @param param Parameter-Struktur mit Event-spezifischen Daten
 */
static void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    // Switch-Case für verschiedene GAP-Events
    switch (event) {
        
    // ========== AUTHENTIFIZIERUNG ABGESCHLOSSEN ==========
    case ESP_BT_GAP_AUTH_CMPL_EVT:{
        
        // Event tritt auf nach Pairing-Prozess (erfolgreich oder fehlgeschlagen)
        
        // Prüfen ob Authentifizierung erfolgreich war
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
            
            // Erfolg: Verbindung ist authentifiziert und sicher
            ESP_LOGI(SPP_TAG, "authentication success: %s", param->auth_cmpl.device_name);
            
            // Zusätzlich: Bluetooth-Adresse des verbundenen Geräts ausgeben
            // Format: XX:XX:XX:XX:XX:XX (6 Bytes in Hexadezimal)
            esp_log_buffer_hex(SPP_TAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
            
        } else {
            
            // Fehler: Pairing fehlgeschlagen
            // Mögliche Gründe: Falscher PIN, Timeout, Abbruch durch Nutzer
            ESP_LOGE(SPP_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }

    // ========== PIN CODE REQUEST (Pairing) ==========
    case ESP_BT_GAP_PIN_REQ_EVT:{
        
        // Event tritt auf wenn Client einen PIN für Pairing anfordert
        // Aktuell wird kein PIN verwendet (Legacy Pairing deaktiviert)
        // Für PIN-Pairing müsste hier esp_bt_pin_reply() aufgerufen werden
        
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
        
        // Beispiel für PIN-Antwort (auskommentiert):
        // if (param->pin_req.min_16_digit) {
        //     esp_bt_pin_code_t pin_code = {0};
        //     esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
        // }
        
        break;
    }

    // ========== SECURE SIMPLE PAIRING (SSP) ==========
    
    /*
     * SSP (Secure Simple Pairing) - Moderner Pairing-Mechanismus
     * 
     * Verschiedene Modi:
     * 1. PASSKEY: Nutzer gibt 6-stellige Zahl ein
     * 2. CONFIRM: Nutzer bestätigt 6-stellige Zahl
     * 3. CONSENT: Einfache Ja/Nein Bestätigung
     * 4. NUMERIC: Numerischer Vergleich
     * 
     * Alle Modi werden hier automatisch bestätigt (für einfache Verbindung)
     */

    case ESP_BT_GAP_CFM_REQ_EVT:
        // CFM = Confirm Request
        // Nutzer soll Pairing-Code bestätigen
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %"PRIu32, param->cfm_req.num_val);
        
        // Automatisch bestätigen (true = akzeptieren)
        esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
        break;
        
    case ESP_BT_GAP_KEY_NOTIF_EVT:
        // KEY NOTIF = Passkey Notification
        // Zeigt den Pairing-Code an (Nutzer muss auf anderem Gerät eingeben)
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%"PRIu32, param->key_notif.passkey);
        break;
        
    case ESP_BT_GAP_KEY_REQ_EVT:
        // KEY REQ = Passkey Request
        // Nutzer soll Pairing-Code eingeben
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        break;

    // ========== UNBEKANNTE/NICHT BEHANDELTE EVENTS ==========
    default: {
        // Alle anderen GAP-Events werden nur geloggt
        ESP_LOGI(SPP_TAG, "event: %d", event);
        break;
    }
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////
// BLUETOOTH SPP CALLBACK
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * SPP Event Callback Funktion
 * 
 * Diese Funktion wird vom Bluetooth-Stack aufgerufen wenn SPP-Events auftreten.
 * SPP (Serial Port Profile) ermöglicht serielle Datenübertragung.
 * 
 * Wichtige Events:
 * - INIT: SPP wurde initialisiert
 * - SRV_OPEN: Client hat sich verbunden
 * - CLOSE: Verbindung wurde getrennt
 * - DATA_IND: Daten wurden empfangen
 * - WRITE: Daten wurden gesendet
 * 
 * @param event Typ des aufgetretenen Events
 * @param param Parameter-Struktur mit Event-spezifischen Daten
 */
static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    // Switch-Case für verschiedene SPP-Events
    switch (event) {
        
    // ========== SPP INITIALISIERUNG ABGESCHLOSSEN ==========
    case ESP_SPP_INIT_EVT:
        
        // Event tritt auf nachdem esp_spp_init() erfolgreich war
        ESP_LOGI(SPP_TAG, "ESP_SPP_INIT_EVT");
        
        // SPP-Server starten
        // Parameter:
        // - sec_mask: Sicherheits-Maske (0 = keine Verschlüsselung)
        // - role_slave: ESP32 ist Server (wartet auf Verbindungen)
        // - 0: Slot/Channel (0 = automatisch wählen)
        // - SPP_SERVER_NAME: Name des SPP-Dienstes
        esp_spp_start_srv(ESP_SPP_SEC_AUTHENTICATE, ESP_SPP_ROLE_SLAVE, 0, SPP_SERVER_NAME);
        break;
        
    // ========== SPP SERVER DISCOVERY ABGESCHLOSSEN ==========
    case ESP_SPP_DISCOVERY_COMP_EVT:
        // Wird aufgerufen nach Service Discovery (wenn ESP32 als Client agiert)
        // In diesem Projekt nicht verwendet (ESP32 ist Server)
        ESP_LOGI(SPP_TAG, "ESP_SPP_DISCOVERY_COMP_EVT");
        break;
        
    // ========== SPP SERVER GEÖFFNET ==========
    case ESP_SPP_OPEN_EVT:
        // Wird aufgerufen wenn ESP32 als Client eine Verbindung öffnet
        // In diesem Projekt nicht verwendet (ESP32 ist Server)
        ESP_LOGI(SPP_TAG, "ESP_SPP_OPEN_EVT");
        break;
        
    // ========== CLIENT HAT SICH VERBUNDEN ==========
    case ESP_SPP_SRV_OPEN_EVT:
        
        // Event tritt auf wenn ein Client sich erfolgreich verbunden hat
        ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_OPEN_EVT");
        
        // Connection Handle speichern für späteres Senden
        // Handle ist eindeutige ID dieser Verbindung
        connection_handle = param->srv_open.handle;
        
        // Handle-Wert ausgeben (für Debugging)
        ESP_LOGI(SPP_TAG, "Connection handle: %"PRIu32, connection_handle);
        break;
        
    // ========== VERBINDUNG WURDE GETRENNT ==========
    case ESP_SPP_CLOSE_EVT:
        
        // Event tritt auf wenn:
        // - Client Verbindung trennt
        // - Verbindung durch Timeout verloren geht
        // - ESP32 Verbindung aktiv trennt
        
        ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT");
        
        // Connection Handle zurücksetzen (keine Verbindung mehr)
        // Verhindert dass weiter Daten gesendet werden
        connection_handle = 0;
        break;
        
    // ========== SPP SERVER GESTARTET ==========
    case ESP_SPP_START_EVT:
        
        // Event tritt auf nachdem SPP-Server erfolgreich gestartet wurde
        ESP_LOGI(SPP_TAG, "ESP_SPP_START_EVT");
        
        // Server-Handle speichern (wird aktuell nicht verwendet)
        // Könnte für Server-Kontrolle genutzt werden
        break;
        
    // ========== CLIENT INITIIERT VERBINDUNG ==========
    case ESP_SPP_CL_INIT_EVT:
        
        // Event tritt auf wenn Client Verbindungsaufbau startet
        ESP_LOGI(SPP_TAG, "ESP_SPP_CL_INIT_EVT");
        break;
        
    // ========== DATEN EMPFANGEN ==========
    case ESP_SPP_DATA_IND_EVT:
        
        // Event tritt auf wenn Client Daten sendet
        
        // Anzahl empfangener Bytes ausgeben
        ESP_LOGI(SPP_TAG, "ESP_SPP_DATA_IND_EVT len=%d", param->data_ind.len);
        
        // Empfangene Daten als Hex-Dump ausgeben (für Debugging)
        // Zeigt rohe Bytes in hexadezimaler Form
        esp_log_buffer_hex("", param->data_ind.data, param->data_ind.len);
        
        // HIER KÖNNTE DATENVERARBEITUNG ERFOLGEN:
        // - Befehle parsen (z.B. "TARE", "RESET")
        // - Konfiguration ändern
        // - Antwort senden
        
        break;
        
    // ========== CONGESTION STATUS ==========
    case ESP_SPP_CONG_EVT:
        
        // Event tritt auf wenn Bluetooth-Buffer voll/leer wird
        // cong = 1: Buffer voll (Senden pausieren)
        // cong = 0: Buffer wieder frei (Senden fortsetzen)
        
        ESP_LOGI(SPP_TAG, "ESP_SPP_CONG_EVT cong=%d", param->cong.cong);
        break;
        
    // ========== DATEN ERFOLGREICH GESENDET ==========
    case ESP_SPP_WRITE_EVT:
        
        // Event tritt auf nachdem esp_spp_write() abgeschlossen ist
        // Bestätigt dass Daten erfolgreich übertragen wurden
        
        // Status prüfen
        if (param->write.status == ESP_SPP_SUCCESS) {
            // Erfolg: Daten wurden gesendet
            ESP_LOGI(SPP_TAG, "ESP_SPP_WRITE_EVT len=%d", param->write.len);
        } else {
            // Fehler beim Senden
            ESP_LOGE(SPP_TAG, "ESP_SPP_WRITE_EVT failed, status=%d", param->write.status);
        }
        break;
        
    // ========== UNBEKANNTE/NICHT BEHANDELTE EVENTS ==========
    default:
        // Alle anderen SPP-Events werden nur geloggt
        ESP_LOGI(SPP_TAG, "SPP event: %d", event);
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// ÖFFENTLICHE FUNKTIONEN
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * Bluetooth initialisieren
 * 
 * Diese Funktion führt die komplette Bluetooth-Initialisierung durch:
 * 1. NVS (Non-Volatile Storage) initialisieren für BT-Konfiguration
 * 2. Bluetooth Controller starten
 * 3. Bluedroid Stack initialisieren
 * 4. GAP und SPP konfigurieren
 * 5. Gerät sichtbar machen
 * 
 * Muss einmal beim Programmstart aufgerufen werden (vor Bluetooth-Nutzung).
 */
void bluetooth_init(void)
{
    // ========== SCHRITT 1: NVS INITIALISIEREN ==========
    
    /*
     * NVS (Non-Volatile Storage) = Flash-Speicher für Konfiguration
     * 
     * Bluetooth speichert hier:
     * - Pairing-Informationen (Bonding)
     * - Link Keys (Verschlüsselungs-Schlüssel)
     * - Gerätename
     * 
     * nvs_flash_init() erstellt NVS-Partition wenn nicht vorhanden
     */
    esp_err_t ret = nvs_flash_init();
    
    // Spezielle Fehlerbehandlung für NVS
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS ist voll oder Version inkompatibel
        // Lösung: Komplett löschen und neu initialisieren
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // ========== SCHRITT 2: BLUETOOTH CONTROLLER INITIALISIEREN ==========
    
    /*
     * Bluetooth Controller = Low-Level Hardware-Treiber
     * 
     * Konfiguriert:
     * - HCI (Host Controller Interface)
     * - RF (Radio Frequency) Parameter
     * - Buffer-Größen
     * - Power Management
     */
    
    // Standard-Konfiguration laden
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    
    // Controller mit Konfiguration initialisieren
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // Controller im Classic Bluetooth Modus starten (nicht BLE!)
    // ESP_BT_MODE_CLASSIC_BT = Nur Classic BT (für SPP)
    // Alternative: ESP_BT_MODE_BTDM = Classic + BLE gleichzeitig
    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // ========== SCHRITT 3: BLUEDROID STACK INITIALISIEREN ==========
    
    /*
     * Bluedroid = Bluetooth Protocol Stack (von Android übernommen)
     * 
     * Implementiert:
     * - L2CAP (Logical Link Control)
     * - SDP (Service Discovery)
     * - RFCOMM (Serial Cable Emulation)
     * - Profile (SPP, A2DP, HFP, etc.)
     */
    
    // Bluedroid Stack initialisieren
    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // Bluedroid Stack aktivieren (startet alle Services)
    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // ========== SCHRITT 4: CALLBACK-FUNKTIONEN REGISTRIEREN ==========
    
    // GAP Callback registrieren (für Verbindungsverwaltung)
    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s gap register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // SPP Callback registrieren (für Datenübertragung)
    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // ========== SCHRITT 5: SPP INITIALISIEREN ==========
    
    /*
     * SPP Mode = VFS (Virtual File System)
     * 
     * Ermöglicht Zugriff auf SPP wie auf serielle Schnittstelle
     * Alternativen:
     * - ESP_SPP_MODE_CB: Callback-basiert (verwendet hier)
     * - ESP_SPP_MODE_VFS: File-Descriptor basiert (für POSIX-Kompatibilität)
     */
    if ((ret = esp_spp_init(ESP_SPP_MODE_CB)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp init failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // ========== SCHRITT 6: SECURE SIMPLE PAIRING KONFIGURIEREN ==========
    
    /*
     * SSP (Secure Simple Pairing) = Moderner Pairing-Mechanismus
     * 
     * IO Capability = Was kann das Gerät anzeigen/eingeben?
     * ESP_BT_IO_CAP_NONE = Keine Ein/Ausgabe (Auto-Pairing)
     * 
     * Alternativen:
     * - ESP_BT_IO_CAP_OUT: Nur Ausgabe (Display)
     * - ESP_BT_IO_CAP_IN: Nur Eingabe (Tastatur)
     * - ESP_BT_IO_CAP_IO: Beides (Display + Tastatur)
     */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_NONE;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));

    // ========== SCHRITT 7: GERÄT SICHTBAR MACHEN ==========
    
    /*
     * Scan Mode = Wie ist Gerät sichtbar?
     * 
     * ESP_BT_CONNECTABLE = Kann verbunden werden
     * ESP_BT_GENERAL_DISCOVERABLE = Für alle sichtbar in Bluetooth-Suche
     * 
     * Ohne diese Einstellung wäre ESP32 "unsichtbar"
     */
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

    /*
     * Gerätenamen setzen
     * 
     * Dieser Name erscheint:
     * - In Bluetooth-Geräteliste auf anderen Geräten
     * - Bei Verbindungsanfrage
     * - In Bluetooth-Einstellungen
     */
    esp_bt_dev_set_device_name(DEVICE_NAME);
    
    // Initialisierung abgeschlossen
    ESP_LOGI(SPP_TAG, "Bluetooth initialisiert als: %s", DEVICE_NAME);
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * Integer-Wert über Bluetooth senden
 * 
 * Sendet einen Integer-Wert als ASCII-String über SPP.
 * 
 * Format: "<wert>\n"
 * Beispiel: "250000\n" (für 2500.00g wenn mit 100 multipliziert)
 * 
 * @param value Integer-Wert der gesendet werden soll
 * 
 * Funktionsweise:
 * 1. Prüfen ob Client verbunden ist (connection_handle != 0)
 * 2. Integer in String konvertieren (mit snprintf)
 * 3. String über SPP senden (mit esp_spp_write)
 * 
 * Sicherheit:
 * - Sendet nur wenn Verbindung aktiv
 * - Buffer-Overflow geschützt (sizeof(buffer))
 * - Automatisches Newline für Paket-Trennung
 */
void bluetooth_send_int(int32_t value)
{
    // Prüfen ob ein Client verbunden ist
    // connection_handle wird bei Verbindung gesetzt, bei Trennung auf 0 zurückgesetzt
    if(connection_handle != 0){
        
        // ========== STRING-FORMATIERUNG ==========
        
        // Buffer für ASCII-String (16 Bytes reichen für int32_t)
        // Maximale Länge von int32_t: "-2147483648" = 11 Zeichen + "\n" + "\0"
        char buffer[16];
        
        // Integer in String konvertieren
        // snprintf = Sichere String-Formatierung (verhindert Buffer-Overflow)
        // Format: "%ld\n"
        //   %ld = long integer (32-bit)
        //   \n  = Newline (Paket-Ende-Markierung)
        // Rückgabe: Anzahl geschriebener Zeichen (ohne \0)
        int len = snprintf(buffer, sizeof(buffer), "%ld\n", (long)value);
        
        // ========== DATEN SENDEN ==========
        
        // Daten über SPP schreiben
        // Parameter:
        //   connection_handle: ID der Verbindung (an welchen Client?)
        //   len: Anzahl zu sendender Bytes
        //   buffer: Pointer auf Daten (als uint8_t* gecastet)
        // 
        // Funktion ist non-blocking (kehrt sofort zurück)
        // Bestätigung erfolgt später über ESP_SPP_WRITE_EVT Callback
        esp_spp_write(connection_handle, len, (uint8_t*)buffer);
        
        // Hinweis: Kein Error-Checking hier
        // Fehler werden im SPP Callback (ESP_SPP_WRITE_EVT) behandelt
    }
    // Wenn nicht verbunden: Daten werden verworfen (kein Senden möglich)
}

// ============================================================================
// ENDE DER BLUETOOTH-IMPLEMENTIERUNG
// ============================================================================
