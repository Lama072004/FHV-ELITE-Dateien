import cv2
import numpy as np
import matplotlib.pyplot as plt

# Lade die Slope Map (Graustufenbild)
slope_map = cv2.imread(r"D:\Mathias Lampert\Documents\Mathias Schule_Studium\FH\EIT_1\WRmP\Python\test\mapimage.png", cv2.IMREAD_GRAYSCALE)

# Überprüfe, ob das Bild korrekt geladen wurde
if slope_map is None:
    print("Fehler: Bild konnte nicht geladen werden!")
else:
    # Normalisiere die Steigungskarte auf den Bereich [0, 255]
    slope_map = cv2.normalize(slope_map, None, 0, 255, cv2.NORM_MINMAX)

    # Höhe berechnen durch schrittweise Integration der Steigungen (kumulative Summe)
    height_map = np.cumsum(slope_map, axis=0)  # Summiere entlang der vertikalen Achse

    # Normalisieren der Höhe auf einen gewünschten Bereich (z.B. 0cm - 200cm)
    height_map = cv2.normalize(height_map, None, 0, 200, cv2.NORM_MINMAX)

    # Speichern der 16-Bit PNG für Blender (maximale Höhe auf 65535 skalieren)
    height_map_16bit = np.uint16(height_map / height_map.max() * 65535)
    cv2.imwrite("heightmap_16bit.png", height_map_16bit)

    # Anzeigen der generierten Heightmap
    plt.imshow(height_map, cmap="gray")
    plt.title("Generierte Heightmap")
    plt.colorbar(label="Höhe (cm)")
    plt.show()

