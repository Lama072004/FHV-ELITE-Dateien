import numpy as np

CC = np.random.random((10, 2)) * 100

X, Y = CC[:,0], CC[:,1]

PC = np.zeros((10, 2))

PC[:,0] = np.sqrt(X**2 + Y**2)
PC[:,1] = np.arctan2(Y,X)

print(f"Kartesiche Koordinaten:\n", CC)
print(f"\n", PC)