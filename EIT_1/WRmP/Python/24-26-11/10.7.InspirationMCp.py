import numpy as np

# Erzeugen einer 10x2-Matrix mit zufälligen kartesischen Koordinaten
cartesian_coords = np.random.rand(10, 2) * 100  # Zufällige Werte zwischen 0 und 100

# Funktion zur Umwandlung von kartesischen in Polarkoordinaten
def cartesian_to_polar(cartesian_coords):
    polar_coords = np.zeros_like(cartesian_coords)
    for i in range(cartesian_coords.shape[0]):
        x, y = cartesian_coords[i]
        r = np.sqrt(x**2 + y**2)
        theta = np.degrees(np.arctan2(y, x))
        polar_coords[i] = [r, theta]
    return polar_coords

# Umwandlung der kartesischen Koordinaten in Polarkoordinaten
polar_coords = cartesian_to_polar(cartesian_coords)

print("Kartesische Koordinaten:\n", cartesian_coords)
print("\nPolarkoordinaten:\n", polar_coords)