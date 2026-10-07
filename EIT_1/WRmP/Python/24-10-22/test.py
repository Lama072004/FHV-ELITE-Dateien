import matplotlib.pyplot as plt
import numpy as np

# Gegebene Kenndaten
peak_voltage = 3.3  # Scheitelwert-Spannung in V
frequency = 3000    # Frequenz in Hz

# Berechnungen
# Periodendauer T (in Sekunden)
T = 1 / frequency

# Effektivwert (RMS) für Sinus: Ueff = U_peak / sqrt(2)
U_eff = peak_voltage / np.sqrt(2)

# Arithmetischer Mittelwert für Sinus über eine vollständige Periode ist 0

# Zeitbereich für eine Periode
time = np.linspace(0, 1.5 * T, 500)  # 1.5 * T to show more than one period
voltage = peak_voltage * np.sin(2 * np.pi * frequency * time)

# Grafik erstellen
plt.figure(figsize=(10, 5))
plt.plot(time * 1000, voltage, label="Sinusspannung")  # Zeit in ms für bessere Darstellung

# Periodendauer einzeichnen und beschriften
plt.axvline(T * 1000, color="gray", linestyle="--", label="Periodendauer T")
plt.text(T * 1000 + 0.1, 0, f"T = {T * 1000:.2f} ms", color="gray", va="center")

# Scheitelwert-Spannung bemaßen
plt.axhline(peak_voltage, color="red", linestyle="--", label="Scheitelwert 3.3 V")
plt.text(0, peak_voltage + 0.2, "3.3 V", color="red")

# Effektivwert (RMS) einzeichnen
plt.axhline(U_eff, color="blue", linestyle="--", label=f"Effektivwert U_eff ≈ {U_eff:.2f} V")
plt.text(0, U_eff + 0.2, f"{U_eff:.2f} V", color="blue")

# Achsen und Titel
plt.title("Sinusspannung mit Scheitelwert 3,3 V und Frequenz 3 kHz")
plt.xlabel("Zeit (ms)")
plt.ylabel("Spannung (V)")
plt.legend(loc="upper right")
plt.grid(True)
plt.ylim(-4, 4)

plt.show()

# Rückgabe der berechneten Werte
T, U_eff, 0  # Arithmetischer Mittelwert für Sinus über eine Periode ist 0
