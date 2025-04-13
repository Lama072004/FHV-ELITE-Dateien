import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Funktion, um die Zeiger des Dreiphasensystems zu berechnen
def get_vectors(t, amplitude=1, frequency=1):
    omega = 2 * np.pi * frequency
    angles = omega * t + np.array([0, -2 * np.pi / 3, 2 * np.pi / 3])  # U, V, W
    return amplitude * np.exp(1j * angles)

# Update-Funktion für die Animation
def update(frame):
    t = frame / 60  # 60 fps
    vectors = get_vectors(t)
    for quiver, vec in zip(quivers, vectors):
        quiver.set_UVC(vec.real, vec.imag)

# Plot einrichten
fig, ax = plt.subplots()
ax.set_xlim(-1.5, 1.5)
ax.set_ylim(-1.5, 1.5)
ax.set_aspect('equal')
ax.grid()

# Pfeile für U, V, W initialisieren
colors = ['r', 'g', 'b']
quivers = [ax.quiver(0, 0, 0, 0, angles='xy', scale_units='xy', scale=1, color=color) for color in colors]

# Animation erstellen
ani = FuncAnimation(fig, update, frames=300, interval=50)  # 300 Frames, ~30 fps
ani.save('three_phase_system.mp4', writer='ffmpeg', fps=30)

plt.show()