Hier ist eine ausführlichere und thematisch gegliederte Zusammenfassung der Inhalte aus dem Dokument **„Halbleitertechnik“**, wie es typischerweise im Studiengang Elektronik und Informationstechnologie (2. Semester, FHV) verwendet wird.

---

## 📘 **Zusammenfassung: Halbleitertechnik (FHV)**

### 1. Einleitung & Bedeutung von Halbleitern

Halbleiter sind Materialien, deren elektrische Leitfähigkeit zwischen der von Leitern und Isolatoren liegt. Durch Dotierung und gezielte Strukturierung lassen sich ihre Eigenschaften steuern – Basis für elektronische Bauelemente wie Dioden, Transistoren, Speicher und Sensoren.

---

### 2. Physikalische Grundlagen

#### 2.1 Mechanik & Quantenphysik

* **Klassische Mechanik**: Begriffe wie Energie, Impuls und Kraft werden eingeführt.
* **Welle-Teilchen-Dualismus**: Elektronen zeigen Wellenverhalten, was zur Beschreibung von Aufenthaltswahrscheinlichkeiten führt (Doppelspaltversuch).
* **Schrödingergleichung**: Grundgleichung der Quantenmechanik zur Beschreibung von Elektronen in Potenzialen.

#### 2.2 Elektromagnetismus

* **Coulomb-Kraft & elektrisches Feld**: Ableitung aus Punktladungen, Beschreibung durch das elektrische Potential $\Phi(\vec{x})$.
* **Poisson-Gleichung**: Verknüpft Ladungsverteilung $\rho(\vec{x})$ mit dem elektrischen Potential.

#### 2.3 Kontinuitätsgleichung

* Beschreibt den Zusammenhang von Stromdichte, Ladung und deren zeitlicher Änderung:

  $$
  \frac{\partial \rho}{\partial t} + \nabla \cdot \vec{j} = 0
  $$

---

### 3. Freie Ladungen & Leitungsmodelle

#### 3.1 Drude-Modell

* Betrachtet Elektronen als freies Gas in einem Gitter aus Atomrümpfen.
* Erklärt Ohm’sches Gesetz, Versagen bei Erklärung von Hall-Effekt, Temperaturabhängigkeit etc.

#### 3.2 Zustandsdichte & Fermi-Funktion

* Zustandsdichte $D(E)$: Verfügbare Quantenzustände je Energieintervall.
* Fermi-Funktion $f(E)$: Wahrscheinlichkeit, dass ein Zustand bei Temperatur $T$ besetzt ist.

#### 3.3 Elektronen als Wellen

* Periodische Randbedingungen → diskrete Wellenvektoren.
* **Fermi-Kugel** im $k$-Raum: Veranschaulichung der erlaubten Elektronenzustände bis zur Fermi-Energie.

---

### 4. Gebundene Ladungen & Bandstruktur

#### 4.1 Potentialtopf & Quanten-Zustände

* Elektronen in endlichen Potenzialen zeigen diskrete Energieniveaus.

#### 4.2 Periodisches Potenzial – Kronig-Penney-Modell

* Führt zu Energiebändern und Bandlücken.
* Erklärt die Entstehung von Leitungs- und Valenzband – Grundlage des Bandmodells von Festkörpern.

---

### 5. Ladungskonzentration & Transport

#### 5.1 Thermisches Gleichgewicht

* Im Gleichgewicht: $n_0 p_0 = n_i^2$
* Dotierung beeinflusst Lage des Fermi-Niveaus.

#### 5.2 Transportmechanismen

* **Driftstrom** durch elektrische Felder: $\vec{j}_{\text{drift}} = q n \mu \vec{E}$
* **Diffusionsstrom** bei Konzentrationsgradienten: $\vec{j}_{\text{diff}} = \pm q D \nabla n$
* **Einstein-Beziehung**: $\frac{D}{\mu} = \frac{kT}{q}$
* **Rekombination/Generation**: Rückkehr zum Gleichgewicht via Lebensdauer $\tau$.

---

### 6. Grundgleichungen der Halbleiterphysik

* **Stromdichte** für Elektronen/Loch:

  $$
  \vec{j}_n = q n \mu_n \vec{E} + q D_n \nabla n,\quad \vec{j}_p = q p \mu_p \vec{E} - q D_p \nabla p
  $$
* **Kontinuitätsgleichungen** für $n, p$ unter Einfluss von Injektion, Rekombination und Stromfluss.
* **Gauss’sches Gesetz**: $\nabla \cdot \vec{E} = \rho / \varepsilon$

---

### 7. Halbleiterbauelemente

#### 7.1 Die Diode

* **pn-Übergang**: Diffusion führt zur Raumladungszone und zur Sperrschichtspannung $U_D$.
* Stromfluss nur bei Vorwärtsspannung: exponentieller Stromverlauf gemäß Shockley-Gleichung:

  $$
  I = I_0 (e^{qU/kT} - 1)
  $$

#### 7.2 Bipolartransistor (BJT)

* Aufbau: npn oder pnp.
* **Stromverstärkung**: Kollektorstrom ist stark vom Basisstrom abhängig.
* Eingesetzt in analogen Schaltungen.

#### 7.3 Feldeffekttransistor (FET)

* **MOSFET**: spannungsgesteuerter Transistor (Ugs kontrolliert Kanal).
* **CMOS-Technologie**: Kombination aus NMOS und PMOS für niedrige Verlustleistung.
* **Subthreshold, Linear- und Sättigungsbereich** werden durch Gate- und Drainspannung definiert.

---

### 8. Halbleiterfertigung

* **Photolithographie**: Belichtung durch Masken, Strukturierung durch Ätzen.
* **Dotierung**: Ionenimplantation oder Diffusion.
* **Oxidschichten, Metallisierung und Planarisierung** als weitere Schritte.
* Herstellung komplexer Chips (z. B. CMOS) mit Featuregrößen < 100 nm.

---

Wenn du möchtest, kann ich:

* eine druckfertige PDF erstellen
* eine Markdown-Datei für GitHub schreiben
* einseitige Formelsammlung aus den Gleichungen generieren

Sag mir einfach Bescheid!
