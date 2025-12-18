# BLE Waage App - Technische Dokumentation

## Überblick

Flutter-App für Android zur Verwaltung einer Bluetooth-Waage (BLE) mit integriertem Rezept-Management und Kochanleitung.

### Hauptfunktionen
1. **Bluetooth-Waage**: Live-Gewichtsmessung via BLE
2. **Rezeptverwaltung**: Erstellen, Bearbeiten, Teilen von Rezepten
3. **Rezept-Follower**: Schrittweise Anleitung mit Gewichtskontrolle
4. **Custom Format**: Proprietäres `.rcpe` Dateiformat zum Teilen

---

## Architektur

### Projekt-Struktur
```
lib/
├── main.dart                    # App-Einstiegspunkt, Navigation
├── models/
│   ├── recipe.dart             # Datenmodell (Recipe, Ingredient)
│   └── recipe.g.dart           # Hive-Adapter (manuell/generiert)
├── screens/
│   ├── scale_screen.dart       # Waage: BLE-Scan & Gewichtsanzeige
│   ├── recipes_screen.dart     # Rezeptliste mit CRUD
│   ├── recipe_editor_screen.dart # Rezept erstellen/bearbeiten
│   ├── recipe_detail_screen.dart # Rezeptansicht
│   └── recipe_follower_screen.dart # Kochanleitung mit Waage
├── widgets/
│   ├── weight_gauge.dart       # Halbkreis-Gauge für Gewicht
│   └── device_list_tile.dart   # BLE-Gerät in Liste
├── services/
│   ├── weight_service.dart     # Zentraler Gewichtsspeicher (ValueNotifier)
│   └── recipe_io_service.dart  # Import/Export von Rezepten
└── theme/
    └── app_theme.dart          # Farbschema & Dark-Theme
```

---

## Kernkomponenten

### 1. BLE-Integration (`scale_screen.dart`)

**Technologie**: `flutter_blue_plus` v1.7.0

**Ablauf**:
1. Bluetooth-Adapter prüfen → Berechtigungen anfordern (Android 12+)
2. Scan starten → Geräte in Liste anzeigen
3. Gerät verbinden (kein Timeout) → Services/Characteristics entdecken
4. Notifications aktivieren (UUID: `0000FF01-...`) → Rohdaten empfangen
5. Gewicht dekodieren (4 Bytes Little-Endian, `/100.0`) → in `WeightService` schreiben

**Wichtige Funktionen**:
- `checkBluetoothAndScan()`: Initialisierung
- `connectDevice()`: Verbindung aufbauen (auto-reconnect bei Trennung)
- `discoverServices()`: Charakteristiken finden & abonnieren
- `_showSnackBar()`: Fehlerausgabe

**UI**: 
- **Scan-Modus**: Liste verfügbarer Geräte mit RSSI
- **Verbunden-Modus**: Gauge (Halbkreis) + digitale Gewichtsanzeige

---

### 2. Gewichts-Gauge (`weight_gauge.dart`)

**Custom Painter**: Zeichnet Halbkreis-Anzeige mit Farbverlauf.

**Modi**:
- **Normal-Modus**: 0 → maxWeight (5kg), Farbe: grün → gelb → rot
- **Rezept-Modus**: Zielgewicht mittig (50%), Toleranzbereich ±5g/5%, Farbverlauf nach Abstand vom Ziel

**Komponenten**:
- Hintergrund-Bogen (grau)
- Aktiver Bogen (farbig, max 180°)
- Tick-Markierungen (11 Striche)
- Toleranz-Arc (grün transparent, Rezept-Modus)
- Ziel-Marker (vertikaler Strich bei 90°)
- Pointer (Kreis am aktuellen Wert)

**Berechnung**:
```dart
// Rezept-Modus: Ziel bei 50%
maxVisual = target * 2.0;
percentage = currentWeight / maxVisual; // 0.0 - 1.0
```

---

### 3. Rezept-Modell (`recipe.dart`)

**Hive TypeAdapter** (manuell erstellt):
```dart
@HiveType(typeId: 0)
class Recipe {
  @HiveField(0) String name;        // Rezeptname
  @HiveField(1) int servings;       // Anzahl Portionen
  @HiveField(2) List<Ingredient> ingredients; // Zutaten
  @HiveField(3) String? description; // Beschreibung
  @HiveField(4) List<String> steps;  // Zubereitungsschritte
}

@HiveType(typeId: 1)
class Ingredient {
  @HiveField(0) String name;   // Zutatname
  @HiveField(1) double amount; // Menge
  @HiveField(2) String unit;   // Einheit (g, ml, Stk, EL, TL)
}
```

**Speicherung**: Hive-Box `recipes` (lokal, persistent)

---

### 4. Rezept-Editor (`recipe_editor_screen.dart`)

**Features**:
- Rezeptname, Portionen, Beschreibung
- Zutaten hinzufügen/löschen (Dialog mit Menge + Einheit)
- Zubereitungsschritte hinzufügen/löschen (nummerierte Liste)
- Speichern in Hive-Box (Update bei bestehendem Index, sonst neu)

**Validierung**: Name muss vorhanden sein

---

### 5. Rezept-Follower (`recipe_follower_screen.dart`)

**Kochanleitung mit Live-Waage**:

1. **Portionsrechner**: Alle Zutaten dynamisch skalieren
2. **Zubereitungsschritte**: Nummerierte Liste (wenn vorhanden)
3. **Zutaten-Checkliste**: 
   - Auswahl per Tap (grüner Rahmen)
   - Gauge zeigt Zielgewicht der ausgewählten Zutat
   - Markieren als erledigt (Button aktiv bei Toleranz ±5g oder für Stück-/Volumen-Einheiten)
   - Bestätigungsdialog bei manueller Markierung außerhalb Toleranz
   - Erledigte Zutaten: grau + Haken

**Toleranz-Logik**:
```dart
isMarkAllowedWithoutWeight(unit):
  // EL, TL, Stk, Tasse, g, ml → sofort markierbar
  // Andere → nur bei Gewicht in Toleranz oder nach Bestätigung
```

**Integration**: `ValueListenableBuilder` auf `WeightService.currentWeight` → live Gauge-Update

---

### 6. Import/Export (`recipe_io_service.dart`)

**Dateiformat `.rcpe`**:
```
[4 Byte] Magic Number: "RCPE"
[1 Byte] Version: 0x01
[64 Char] SHA256-Checksumme (Hex-String)
[Rest]    JSON-Payload (Recipe)
```

**Export**:
1. Recipe → Map → JSON
2. SHA256-Checksumme berechnen
3. Header + Checksumme + JSON kombinieren
4. Speichern in Downloads/Dokumente (Android)

**Import**:
1. Magic Number prüfen
2. Version validieren
3. Checksumme verifizieren (Tamper-Protection)
4. JSON → Recipe parsen

**Speicherort-Dialog** (vereinfacht):
- Standard: `/storage/emulated/0/Download`
- Optional: Dokumente-Ordner

---

### 7. Gewichts-Service (`weight_service.dart`)

**Singleton ValueNotifier**:
```dart
class WeightService {
  static final ValueNotifier<double?> currentWeight = ValueNotifier(null);
  
  static void setWeight(double? w) => currentWeight.value = w;
  static double? getWeight() => currentWeight.value;
}
```

**Zweck**: Zentraler Speicher für aktuelles Gewicht → von `scale_screen` geschrieben, von `recipe_follower_screen` gelesen

---

## UI/UX-Design

### Theme (`app_theme.dart`)

**Farbpalette**:
```dart
darkGreen:   #386641 (AppBar, Navigation)
green:       #6A994E (Primary, Icons)
lightGreen:  #A7C957 (Akzente, Hervorhebungen)
cream:       #F2E8CF (Text-Highlights, Marker)
red:         #BC4749 (Fehler, Löschen)
```

**Dark-Theme**: `scaffoldBackgroundColor: #121212`, Surfaces: `#1E1E1E`

### Navigation

**Hauptscreen** (`main.dart`):
- Portrait: `NavigationBar` unten (2 Tabs)
- Landscape: `NavigationRail` rechts (gleichmäßiger Abstand)

**Tabs**:
1. Waage (Icon: `scale`)
2. Rezepte (Icon: `menu_book`)

---

## Datenfluss

### Gewichtsmessung
```
BLE-Gerät → Notification (4 Bytes)
  → scale_screen.discoverServices()
    → Dekodierung (Little-Endian, /100)
      → WeightService.setWeight()
        → ValueNotifier → recipe_follower_screen (Gauge-Update)
```

### Rezept-Speicherung
```
Editor → Recipe-Objekt
  → Hive.box<Recipe>('recipes').add()/putAt()
    → Hive-Adapter (recipe.g.dart) serialisiert
      → Lokale Datei (Hive-Box)
        → ValueListenableBuilder in recipes_screen aktualisiert UI
```

### Rezept-Export
```
Recipe → RecipeIOService.exportRecipe()
  → JSON + SHA256-Checksumme + Magic-Header
    → .rcpe-Datei in Downloads
      → Share-Intent (manuell via Dateisystem)
```

---

## Abhängigkeiten

### Runtime
- `flutter_blue_plus: ^1.7.0` – BLE-Kommunikation
- `permission_handler: ^12.0.1` – Android-Berechtigungen
- `hive: ^2.2.3` + `hive_flutter: ^1.1.0` – Lokale Datenbank
- `path_provider: ^2.1.0` – Dateipfade (Downloads)
- `crypto: ^3.0.3` – SHA256-Hashing

### Dev
- `hive_generator: ^2.0.0` – Adapter-Generator
- `build_runner: ^2.4.0` – Code-Generierung
- `flutter_launcher_icons: ^0.13.1` – Icon-Generierung

---

## Build & Deployment

### Debug-Build
```bash
flutter clean
flutter pub get
dart run build_runner build --delete-conflicting-outputs
flutter run -d <device-id>
```

### Release-APK
```bash
flutter build apk --release
# Output: build/app/outputs/flutter-apk/app-release.apk
```

### Installation
```bash
adb install -r build/app/outputs/flutter-apk/app-release.apk
```

**Hinweis**: Pfad darf keine Leerzeichen enthalten (Gradle-Limitation).

---

## Berechtigungen (Android)

### Manifest (`android/app/src/main/AndroidManifest.xml`)
```xml
<uses-permission android:name="android.permission.BLUETOOTH" />
<uses-permission android:name="android.permission.BLUETOOTH_ADMIN" />
<uses-permission android:name="android.permission.BLUETOOTH_SCAN" />
<uses-permission android:name="android.permission.BLUETOOTH_CONNECT" />
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
```

**Runtime**: `permission_handler` fordert bei App-Start an.

---

## Bekannte Einschränkungen

1. **Import-Dialog**: Kein File-Picker (file_picker entfernt wegen Build-Konflikten) → manuell via Dateisystem
2. **iOS-Support**: Nicht getestet (nur Android optimiert)
3. **Rezept-Schritte**: Nur Text, keine Bilder/Timer
4. **Persistenz**: Erledigte Zutaten im Follower werden nicht gespeichert (nur Session-State)
5. **Netzwerk**: Keine Cloud-Sync (alles lokal)

---

## Erweiterungsmöglichkeiten

- [ ] Cloud-Backup (Firebase/Supabase)
- [ ] Bilder für Rezepte
- [ ] Timer für Zubereitungsschritte
- [ ] Barcode-Scanner für Zutaten
- [ ] Multi-Device-Sync
- [ ] Rezept-Community (Share-Platform)
- [ ] Sprach-Unterstützung (i18n)

---

## Debugging

### BLE-Probleme
```dart
// In scale_screen.dart:
print('Services found: ${services.length}');
print('Characteristic UUID: ${c.uuid}');
```

### Hive-Box prüfen
```dart
// In recipes_screen.dart:
print('Rezepte in Box: ${recipesBox.length}');
```

### Gewicht-Live-Log
```dart
// In weight_service.dart:
setWeight(double? w) {
  print('Weight updated: $w g');
  currentWeight.value = w;
}
```

---

## Kontakt & Lizenz

**Entwickler**: Mathias Lampert  
**Projekt**: ELITE-Semester EIT_3, Embedded Systems  
**Version**: 1.0.0  
**Lizenz**: Private (nicht veröffentlicht)

---

*Letzte Aktualisierung: 2024*