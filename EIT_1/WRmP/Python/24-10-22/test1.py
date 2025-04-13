import matplotlib.pyplot as plt
import numpy as np

# Vektoren erstellen (Winkel in Bogenmaß)
vector_a = np.array([0, -2 * np.pi / 3, 2 * np.pi / 3])

# Länge der Vektoren
vector_magnitudes = np.ones_like(vector_a)  # Einheitliche Länge

# Ursprüngliche kartesische Koordinaten berechnen
x_components = vector_magnitudes * np.cos(vector_a)  # x-Koordinaten
y_components = vector_magnitudes * np.sin(vector_a)  # y-Koordinaten

# Rotationswinkel (in Bogenmaß)
rotation_angle = np.pi / 6  # Beispiel: 30 Grad gegen den Uhrzeigersinn

# Rotationsmatrix definieren
rotation_matrix = np.array([
    [np.cos(rotation_angle), -np.sin(rotation_angle)],
    [np.sin(rotation_angle), np.cos(rotation_angle)]
])

# Neue Koordinaten berechnen
rotated_vectors = np.dot(rotation_matrix, np.vstack((x_components, y_components)))

# Rotierte x- und y-Komponenten
x_rotated = rotated_vectors[0]
y_rotated = rotated_vectors[1]

# Plot einrichten
fig, ax = plt.subplots()
ax.set_xlim(-1.5, 1.5)
ax.set_ylim(-1.5, 1.5)
ax.set_aspect('equal')
ax.grid()

# Ursprüngliche Vektoren plotten (grau)
for x, y in zip(x_components, y_components):
    ax.quiver(0, 0, x, y, angles='xy', scale_units='xy', scale=1, color='gray', alpha=0.5)

# Rotierte Vektoren plotten (blau)
for x, y in zip(x_rotated, y_rotated):
    ax.quiver(0, 0, x, y, angles='xy', scale_units='xy', scale=1, color='blue')

# Ursprung markieren
ax.plot(0, 0, 'ro')  # Ursprungspunkt

plt.title("Vektoren und ihre Rotation")
plt.show()
