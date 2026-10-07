import numpy as np
import time
p = 8 # exponent
a = np.random.randn(10**p)
b = np.random.randn(10**p)
t = time.time() # Return the current time in seconds
s = np.dot(a,b)
elapsed = time.time() - t # time in seconds needed to compute the dot product
print(f"Elapsed time: {elapsed:.2f} s")