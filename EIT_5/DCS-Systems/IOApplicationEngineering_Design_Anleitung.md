# IOApplicationEngineering – Design Phase (Schritt-für-Schritt)

Quellen: IOApplicationEngineering.pdf + DNA-instructions-in-a-nutshell.pdf (FbCAD-Regeln, am/mtr/mgv, Direct Access Ports) + Connection_example.pdf (I/O-Verkabelung)

---

## Vorbereitung (aus IOApplicationEng.)
- FbCad öffnen
- Package = GP01, Execution Time = 1000 ms, Order = 20
- Loop-Tag = TAG_CODE (später Global Edit)
- Jeder Objektname eindeutig + "pr:"-Präfix; Workspace = z.B. TSA20

---

## 1. Temperaturmessung Pt100 (aus IOAppEng. + DNA-instructions)
- I/O → ACN80 → AI8 → Input-Modul: `pr:xxxx-TT-100.I`
- Analog-Modul `am` (Fblocks1/am, Version lt. Figur 1): `pr:xxxx-TT-100`
- Alarmlimits HH/H aus Common/Constant
- DNA-Check: `am` vergleicht `:av` mit Limits, setzt Alarme

## 2. Digitale Inputs DI8 (IOAppEng.)
- I/O → ACN M80 → DI8, `FBC slot=2`, `IBC=0`, `Card place=0`, Channel `0`
- Zweiter DI8: gleich, nur Channel `1`

## 3. Direct Access Port (IOAppEng. + DNA-instr.)
- Common/Direct Access Port → `xxxx-HS-1`, Initialwert `0`
- Global im gesamten System → Name zwingend einzigartig
- Verbindet zur Bildschirm-Schaltfläche (Beacon/Signallicht)

## 4. Digital Output DO8 (IOAppEng.)
- I/O → ACN M08 → DO → do8
- `FBC slot=2`, `IBC=0`, `Card place=2`, `Channel=0`

## 5. Verbindungen (IOAppEng. + DNA-instr.)
- `Draw/Scalar autocolor wire`
- Output-Ports (`:out`, `:DI0_0`, `:DI0_1`) = Interface-Ports für andere Loops

## 6. Check & Speichern (IOAppEng.)
- Save As → Workspace (EAS)
- File → Check → Fehler im Check-Log behoben
- **Nur speichern, nicht herunterladen/Repo – nur im Labor!** (vorgegeben)

---

## Cross-Check mit übrigen Dateien im Verzeichnis
- **Connection_example.pdf**: I/O geht über Cross-Connection (JAMAK/NOMAK) von Feldgerät zu ACN I/O; hier nur Software-Seite (FbCAD-Module) abgebildet
- **DNA-instructions-in-a-nutshell.pdf**: Bestätigt `am`, `mtr`, `mgv`, `pid`, `ccoX`; Direct Access Ports müssen global eindeutig sein; Interfaces `in/out` nur einmal pro Diagramm
- **Valmet DNA short overview.pdf**: ACN-Controller/I/O-Architektur bestätigt (ACN IO, ACN M80/08, Prozessstation GP01)
- **Function Test tool manual.pdf**: Testing erst im Labor (nicht Teil dieses Design-Steps)
