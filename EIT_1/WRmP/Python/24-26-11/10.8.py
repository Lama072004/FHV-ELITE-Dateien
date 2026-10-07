#Lassen Sie ein Python-Skript die Minimal- und die Maximalwerte der verfügbaren Ganzzahlund Fließkommazahlbereiche ausgeben.
#Bei den Fließkommazahlbereichen geben Sie zusätzlich die kleinste noch feststellbare Differenz benachbarter Werte aus.
import numpy as np

print("Für Float32")
float_info= np.finfo(np.float32)

float_min_32 = float_info.min
float_max_32 = float_info.max
klD_float_32 = float_info.eps

print(f"Minimalwert\n",float_min_32)
print(f"Maximalwert\n",float_max_32)
print(f"kleinste noch feststellbare Differenz\n",klD_float_32)

print("Für Float64")
float_info= np.finfo(np.float64)

float_min_64 = float_info.min
float_max_64 = float_info.max
klD_float_64 = float_info.eps

print(f"Minimalwert\n",float_min_64)
print(f"Maximalwert\n",float_max_64)
print(f"kleinste noch feststellbare Differenz\n",klD_float_64)