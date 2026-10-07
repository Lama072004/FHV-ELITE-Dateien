import sympy as sp
from math import *

d_R = 0.100
m_L = 3
mu = 0.3
F_R_B = 40
V = 0.5
T_a = 1
U = 24

g = 9.81

n= 60*V/d_R/pi
F_al=m_L*V/T_a
F_al, "N"
F_L = mu * m_L*g+F_R_B
F_L, "N"

M_max = F_L*d_R/2 + F_al*d_R/2
M_max, "Nm"
print(M_max)