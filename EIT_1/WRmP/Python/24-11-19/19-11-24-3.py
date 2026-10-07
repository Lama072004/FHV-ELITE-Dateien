import sympy as sp

v1 = 0.01
USS = 0.006
Schallgeschw = 343.2

S = (USS * Schallgeschw)/2
print(f"{S} Meter")

S1 = v1 * USS
print(f"{S1} Meter")

Sgenau = S - S1
print(f"{Sgenau} Meter")
