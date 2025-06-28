## 1. Si-Diode und Gleichrichtung

**1. Skizzieren Sie die typische U-I-Kennlinie einer Si-Diode.**

Die U-I-Kennlinie einer Siliziumdiode zeigt:

* Im Durchlassbereich (U > 0,6–0,7 V): exponentieller Stromanstieg
* Im Sperrbereich (U < 0): nahezu kein Strom (Sperrstrom)
* Im Durchbruchbereich (U sehr negativ): starker Stromanstieg

```
            |
       I    |           / 
            |         /
            |_______/       <- Durchlassbereich (ab ca. 0.7V)
            |
            |        .       <- Sperrbereich mit minimalem Strom
            |______________________ U
```

**2. Bestimmen Sie aus der Kennlinie Abbildung 5.1a für den Punkt P den Gleichstromwiderstand RD und den differenziellen Widerstand rD.**

* Gleichstromwiderstand: $R_D = \frac{U_P}{I_P}$
* Differenzieller Widerstand: $r_D = \left. \frac{dU}{dI} \right|_P$ (Steigung an Punkt P)

**3. Zeichnen Sie in einem Diagramm die Verläufe des Stromes IF und der Spannung U2 in Abhängigkeit von der Zeit.**

* U1: Sinusspannung
* U2: Einweggleichrichtung (nur positive Halbwellen sichtbar)
* IF: fließt nur während positiver Halbwellen

**4. Wie arbeitet die Brückengleichrichterschaltung (Bild 5.2)?**

* Vier Dioden leiten wechselweise, sodass immer dieselbe Polarität an Ua anliegt.
* Ua: pulsierende Gleichspannung (positive Halbwellen)

**5. Wie kann man feststellen, ob eine Diode funktionsfähig oder defekt ist?**

* Mit Multimeter im Diodentestmodus:

  * Durchlassrichtung: ca. 0,6–0,7 V
  * Sperrrichtung: kein Durchgang (OL)

**6. Logikschaltung mit Dioden (Bild 5.1b):**

* UF = 0.7 V, UEH = 12V, UEL = 0V
* Ausgangsspannung = min(der Eingangsspannungen - 0.7 V)
* Logische Funktion: AND-Gatter (mit Diodenlogik realisiert)

**7. Zenereffekt:**

* Elektronen tunneln bei starker Feldstärke im Sperrbetrieb durch das dünne p-n-Gebiet (typisch <5V)

**8. Lawineneffekt (Avalanche):**

* Bei höherer Sperrspannung (>5V) werden Elektronen durch Stoßionisation beschleunigt -> Lawineneffekt

**9. Verhalten im Durchlassbereich:**

* Wie normale Diode, leitet ab ca. 0.7 V (Si)

**10. Skizze Z-Diodenkennlinie:**

* Linker Bereich: Durchbruchspannung (UZ)
* Rechter Bereich: Durchlassbereich wie normale Diode

**11. Strombegrenzung im Durchbruchsbereich nötig, weil:**

* Ohne Vorwiderstand würde Z-Diode zerstört (Wärmeentwicklung)

**12. Arbeitsbereich markieren:**

* Im Durchbruchsbereich mit Strombegrenzung durch Vorwiderstand

**13. Differenzieller Zenerwiderstand:**

* $r_z = \left. \frac{dU}{dI} \right|_{UZ}$

**14. Temperaturkoeffizienten Erklärung:**

* Zenereffekt (UZK < 6V): negativer Temperaturkoeffizient
* Lawineneffekt (UZK > 6V): positiver Temperaturkoeffizient
* Bei ca. 5.6 V kompensieren sich beide

---

## 2. Bipolartransistor (BJT)

**1. Skizzieren Sie den Schichtaufbau eines npn-Transistors.**

* Aufbau: n – p – n
* Anschlüsse:

  * Emitter (n)
  * Basis (p)
  * Kollektor (n)

**2. Schnitt durch npn-Transistor und Polarität:**

* BE-Strecke: Durchlassrichtung
* BC-Strecke: Sperrrichtung

**3. Drei Spannungen und Ströme:**

* Spannungen: UBE, UCE, UCB
* Ströme: IB, IC, IE

**4. Beziehungen:**

* $I_E = I_B + I_C$
* $U_{CE} = U_{CB} + U_{BE}$

**5. Eingangskennlinie & rBE:**

* Eingangskennlinie: Stromverlauf IB über UBE
* $r_{BE} = \left. \frac{dU_{BE}}{dI_B} \right|_A$

**6. Ausgangskennlinienfeld & rCE:**

* IC über UCE bei konstantem IB
* $r_{CE} = \left. \frac{dU_{CE}}{dI_C} \right|_A$

**7. Differentieller Stromverstärkungsfaktor β:**

* $\beta = \frac{\Delta I_C}{\Delta I_B}$

**8. Beispiel: β = 120:**

* IC ist 120-mal größer als IB

**9. Restströme und Indizes:**

* ICB0, ICE0 – Rückwärts- und Sperrströme

**10. Sperr- und Durchbruchspannungen:**

* Sperrspannung: Grenze des normalen Sperrbetriebs
* Durchbruch: kritische Spannung, ab der Zerstörung möglich

**11. Verlustleistung:**

* $P = U_{CE} \cdot I_C$

**12. SOA (Safe Operating Area):**

* Begrenzung durch max. Spannung, Strom, Verlustleistung

---

## 3. Arbeitspunkt & Gegenkopplung

**1. Linearer Bereich im Ausgangskennlinienfeld:**

* Zwischen Sättigungs- und Sperrbereich, IC abhängig von IB

**2. Motivation Arbeitspunkteinstellung:**

* Konstantes Verstärkungsverhalten, temperaturunabhängig

**3. Nachteil Vorwiderstand:**

* Temperatur- und Exemplarabhängigkeit

**4. Vorteil Spannungsteiler:**

* Stabilerer Arbeitspunkt, unabhängig von β

**5. Gleichstrom-Gegenkopplung – Zweck:**

* Stabilisierung des Arbeitspunkts

**6. Gleichstrom-Gegenkopplung – Funktion:**

* Spannungsabfall am Emitterwiderstand senkt BE-Spannung bei höherem IC → Gegenkopplung

**7. Wechselstrom-Gegenkopplung – Zweck:**

* Begrenzung der Verstärkung, Verbesserung Linearität

**8. Wechselstrom-Gegenkopplung – Funktion:**

* Spannungsteiler oder Emitterwiderstand + Kondensator

**9. Verstärkungsarten:**

* Gleichstromverstärkung: Verhältnis von IC zu IB
* Wechselstromverstärkung: Verhältnis ΔIC zu ΔIB
* Betriebsverstärkung: tatsächliche Verstärkung mit Signalgrößen

**10. Hochpässe bei Emitterschaltung:**

* 3 Stück:

  * Koppelkondensator Eingang
  * Koppelkondensator Ausgang
  * Emitterkondensator (um Gegenkopplung zu umgehen)

**11. Untere Grenzfrequenz:**

* Frequenz, bei der Verstärkung auf 70,7 % fällt

**12. Anwendungen der Grundschaltungen:**

* Emitterschaltung: Spannungsverstärker
* Kollektorschaltung: Impedanzwandler
* Basisschaltung: Hochfrequenztechnik

---

## 4. Transistor als Schalter

**1. Vorteile gegenüber mechanischem Schalter:**

* Schnell
* Verschleißfrei
* Integrierbar

**2. AP Sperrzustand (Bild 24.1):**

* UCE hoch, IC ≈ 0

**3. AP Sättigungszustand:**

* UCE ≈ 0.2 V, IC maximal

**4. Basisstromdimensionierung:**

* Mindestens $I_B = \frac{I_C}{\beta} \cdot 2$

**5. Ausgangskennlinienfeld mit ohmscher Last:**

* Gerade mit positiver Steigung von Ursprung

**6. Safe-Operating-Area bei ohmscher Last:**

* Kurzer Überschreiten nicht kritisch → thermische Trägheit

**7. Kennlinienfeld mit kapazitiver Last:**

* Stromverzögerung beim Ein- und Ausschalten

**8. Kennlinienfeld mit induktiver Last:**

* Spannungsspitze beim Ausschalten

**9. Problem ohne Freilaufdiode:**

* Überspannung zerstört Transistor

**10. Schutz nötig weil:**

* Induktionsspannung hoch, führt zu Durchbruch

**11. Schutzschaltung:**

* Freilaufdiode antiparallel zur Last

**12. Schaltverzögerung:**

* Speicherzeit + Anstieg/Abfallzeit (Ladungsträgereinfluss)

**13. Schaltzeiten reduzieren:**

* Basiswiderstand mit Pull-down
* Zusatztransistor (aktive Entladung)

---

## 5. Feldeffekttransistor (FET)

**1. Ladungsträger:**

* BJT: Majoritätsladungsträger + Minderheiten
* FET: nur Majoritätsladungsträger

**2. Steuerung BJT:**

* Stromgesteuert (IB)

**3. Steuerung FET:**

* Spannungssteuerung (UGS)

**4. Anwendungen:**

* BJT: analoge Verstärkung
* FET: digitale Schaltung, hohe Eingangsimpedanz

---

## 6. JFET

**1. Symbol n-Kanal:**

* Pfeil zeigt *hinein* (Gate)

**2. Symbol p-Kanal:**

* Pfeil zeigt *heraus* (Gate)

**3. Verhalten n-Kanal, G auf Masse, D>S:**

* Stromfluss von D nach S, Verarmung durch G-Spannung

---

## 7. IGFET / MOSFET

**1. Aufbau N-Kanal Anreicherungstyp:**

* Substrat p, n-Kanal durch UGS > Uth

**2. Aufbau N-Kanal Verarmungstyp:**

* n-Kanal vorhanden, wird durch negative UGS abgesperrt

**3. Symbol N-Kanal, selbstsperrend:**

* Keine Linie durch Kanal, Pfeil zum Kanal

**4. Symbol P-Kanal, selbstsperrend:**

* Pfeil vom Kanal weg

**5. Symbol N-Kanal, selbstleitend:**

* Linie durch Kanal (initial leitend)

**6. Symbol P-Kanal, selbstleitend:**

* Linie durch Kanal + Pfeil vom Kanal weg

**7. Verhalten N-Kanal, UGS = 0, UDS > 0:**

* Kein Stromfluss → gesperrt

**8. Verhalten bei UGS > Uth:**

* Kanal bildet sich, Stromfluss möglich

**9. Unterschied selbstsperrend vs. selbstleitend:**

* Selbstsperrend: leitet nur bei UGS > Uth
* Selbstleitend: leitet ohne Spannung, Sperrung durch UGS < 0

**10. Unterschied Signal- und Leistungs-MOSFET:**

* Leistungs-MOSFET: größere Ströme, integrierte Schutzstrukturen

**11. Kapazität für Miller-Plateau:**

* Gate-Drain-Kapazität

**12. Parameter zur Beeinflussung:**

* Gate-Widerstand / Treiberleistung

**13. ESD-Schutzmaßnahmen:**

* Schutzdioden, Gate-Widerstand, ESD-Armband

**14. Verlustleistungen als Schalter:**

* Schaltverluste + Leitverluste (RDS(on))

**15. Treiberschaltungen notwendig weil:**

* Hohes C\_GS → schneller Ladestrom nötig

**16. Emitterschaltung als Pegelwandler:**

* Wandelt Logikpegel, treibt Last mit mehr Strom

**17. Nachteil Emitterschaltung als Treiber:**

* Spannungsverlust durch V\_BE, langsamer als CMOS-Treiber
