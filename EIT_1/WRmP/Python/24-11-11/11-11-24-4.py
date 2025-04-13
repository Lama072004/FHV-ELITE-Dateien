import numpy as np

A = np.array([[0,2,-1], [2,3,1], [6,42,8]])
print(A)
print(A.shape)
B = A[-1,:]
print(B)
C = A[:,1]
print(C)