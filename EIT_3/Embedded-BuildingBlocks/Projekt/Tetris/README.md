# Tetris
Tetris für Embedded Systems Building Blocks im Rahmen vom Studium EIT-DU

# 🧩 Minimalistisches Tetris für den ESP32-S3

## 📌 Projektziel
Entwicklung eines minimalistischen **Tetris-Spiels** für den **ESP32-S3**, bei dem die Spielsteine nur **nach links und rechts** bewegt werden können.  
Die Ausgabe erfolgt wahlweise auf:
- einem **OLED-Display** (SSD1306, I²C),
- einer **Flutter-App** auf dem Smartphone (BLE),
- oder einer **LED-Matrix** (über BLE oder Wi-Fi).

Ziel ist die Anwendung **objektorientierter Prinzipien in C** auf einem Embedded-System.

---

## 🧰 Hardware

- 🧠 **Mikrocontroller:** ESP32-S3  
- ⬅️➡️ **Eingabe:** Tasten für „Links“, „Rechts“, „Start/Pause“  
- 🖥️ **Ausgabe-Option 1:** OLED-Display (SSD1306) via I²C  
- 📱 **Ausgabe-Option 2:** Flutter-App (BLE)  
- 🔊 **Optional:** Summer für Soundeffekte

---

## 💻 Software / Programmiersprachen

- **Programmiersprache:** C (objektorientierter Stil mit Structs & Funktionszeigern)  
- **Toolchain:** ESP-IDF  
- **Flutter-App:** Darstellung des Spielfelds auf Smartphone (Steuerung optional)

---

## 🕹️ Spielkonzept

1. **Spielfeld:** 10 × 20 Raster  
2. **Steine:** Standard-Tetris-Blöcke *(ohne Rotation)*  
3. **Bewegung:** Nur horizontal (links/rechts)  
4. **Fallgeschwindigkeit:** Automatischer Fall der Steine  
5. **Reihen löschen:** Vollständig gefüllte Reihen werden entfernt  
6. **Game Over:** Wenn neue Steine nicht mehr ins Feld passen

---

## 🧱 Objektorientierte Struktur (in C)

### `Block` (Piece)
- **Attribute:** Position, Form, Farbe  
- **Methoden:** `moveLeft()`, `moveRight()`, `fall()`

### `Board` (Spielfeld)
- **Attribute:** Rastermatrix, Punktestand  
- **Methoden:** `checkCollision()`, `clearLines()`, `updateDisplay()`

### `Game` (Spiel)
- **Attribute:** aktueller Block, nächster Block, Spielstatus  
- **Methoden:** `spawnBlock()`, `update()`, `handleInput()`

---

## 🖼️ Ausgabeoptionen

- **OLED:** Direkte Pixelmanipulation über I²C, einfache Animation  
- **Flutter-App:** ESP32 sendet Spielfeldzustand via BLE, App rendert das Spiel auf dem Smartphone  
- *(Optional)* **LED-Matrix:** Anzeige über BLE oder Wi-Fi

---

## 🧠 Besondere Lernziele

- Objektorientierte Programmierung in C  
- Echtzeitsteuerung & Animation auf Embedded-Hardware  
- BLE-Kommunikation zwischen ESP32 und Smartphone  
- Speicher- & Ressourcenmanagement

---

## 🚀 Erweiterungsmöglichkeiten

- Anzeige des Punktestands  
- Schwierigkeitsstufen (z. B. erhöhte Fallgeschwindigkeit)  
- Highscore-Speicherung im Flash  
- Soundeffekte über Summer  
- LED-Matrix als alternative Ausgabe

---

## 🧾 Lizenz

Dieses Projekt steht unter einer freien Lizenz.
