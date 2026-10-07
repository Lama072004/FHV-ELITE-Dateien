import matplotlib.pyplot as plt
import numpy as np

# Vektoren erstellen (Winkel in Bogenmaß)
vector_a = np.array([0, -2 * np.pi / 6, 2 * np.pi / 6])

# Länge der Vektoren
vector_magnitudes = np.ones_like(vector_a)  # Einheitliche Länge

# Umwandlung in kartesische Koordinaten (x, y) für den Plot
x_components = vector_magnitudes * np.cos(vector_a)  # x-Koordinaten
y_components = vector_magnitudes * np.sin(vector_a)  # y-Koordinaten

# Plot einrichten
fig, ax = plt.subplots()
ax.set_xlim(-1.5, 1.5)
ax.set_ylim(-1.5, 1.5)
ax.set_aspect('equal')
ax.grid()

# Vektoren plotten mit zugehörigen Farben
colors = ['r', 'g', 'b']  # Farben für die Vektoren
for x, y, color in zip(x_components, y_components, colors):
    ax.quiver(0, 0, x, y, angles='xy', scale_units='xy', scale=1, color=color)

plt.show()
