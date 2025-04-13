import sympy as sp
from sympy.abc import x
print( sp.integrate( 2*x**2, x))
# Unbestimmtes Integral
print( sp.integrate( 2*x**2, (x, -2, 2)))
# Bestimmtes Integral
print( sp.integrate( sp.exp( -x), (x, 0, sp.oo)))
# Auch das geht natürlich