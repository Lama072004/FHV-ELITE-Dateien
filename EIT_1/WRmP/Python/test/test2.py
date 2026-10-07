import cv2
import numpy as np
import matplotlib.pyplot as plt

# Pfad zur Slope Map
slope_map_path = r"D:\Mathias Lampert\Documents\Mathias Schule_Studium\FH\EIT_1\WRmP\Python\test\mapimage.png"

# Slope Map laden (als Graustufenbild)
slope_map = cv2.imread(slope_map_path, cv2.IMREAD_GRAYSCALE)
if slope_map is None:
    raise FileNotFoundError(f"Die Datei '{slope_map_path}' konnte nicht geladen werden.")

# Normalisiere die Graustufenwerte auf den Bereich [0, 1]
slope_map_normalized = slope_map / 255.0

# Initialisiere die Heightmap mit Nullwerten
height_map = np.zeros_like(slope_map_normalized, dtype=np.float32)

# Pixelweise Integration von oben nach unten
for y in range(1, slope_map.shape[0]):  # Beginne bei der zweiten Zeile (y=1)
    for x in range(slope_map.shape[1]):  # Gehe durch jede Spalte
        height_map[y, x] = height_map[y - 1, x] + slope_map_normalized[y, x]

# Skaliere die Heightmap auf den gewünschten Höhenbereich (0 m unten, 2 m oben)
# Finde die Höhe mittig oben und mittig unten
mid_x = slope_map.shape[1] // 2
height_at_top = height_map[0, mid_x]  # Höhe oben (erste Zeile)
height_at_bottom = height_map[-1, mid_x]  # Höhe unten (letzte Zeile)

# Skaliere die Heightmap, sodass mittig oben 2 m und mittig unten 0 m ist
scale_factor = 2.0 / (height_at_top - height_at_bottom)
height_map = (height_map - height_at_bottom) * scale_factor

# Speichere die Heightmap als 16-Bit PNG (für Blender)
heightmap_16bit = np.uint16(height_map / height_map.max() * 65535)
output_path = r"D:\Mathias Lampert\Documents\Mathias Schule_Studium\FH\EIT_1\WRmP\Python\test\heightmap_16bit.png"
cv2.imwrite(output_path, heightmap_16bit)

# Zeige die Heightmap an
plt.imshow(height_map, cmap="gray", vmin=0, vmax=2)
plt.title("Generierte Heightmap (von oben nach unten integriert)")
plt.colorbar(label="Höhe (m)")
plt.show()

print(f"Heightmap wurde erfolgreich erstellt und unter '{output_path}' gespeichert.")