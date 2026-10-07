import numpy as np
import os
import math
import matplotlib.pyplot as plt

n = 500
x_min, x_max = -2 * math.pi, 2 * math.pi
x = np.linspace(x_min, x_max, n)
y = np.sin(x)**2
plt.plot(x,y)
plt.show()