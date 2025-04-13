import numpy as np

X = np.random.random((10))*10
print(f"X\n",X)
Y = np.random.random((10))*10
print(f"Y\n",Y)
CC = [X, Y]
R1 = np.sqrt(X**2 + Y**2)
R2 = np.arctan2(Y,X)
PC = [R1, R2]

print("CC\n",CC)
print("PC\n",PC)

stackedCC = np.column_stack((X,Y))
stackedPC = np.column_stack((R1, R2))
print("CC",stackedCC)
print("PC",stackedCC)