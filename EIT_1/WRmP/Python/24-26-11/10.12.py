import sympy as sp
from sympy.abc import x
i = sp.integrate( sp.exp( -(x**2)), (x, -sp.oo, sp.oo))
print( i)
import sympy as sp
integral = sp.Integral( sp.exp( -(x**2)), (x, -sp.oo, sp.oo))
print( integral.evalf())
import sympy as sp
f = sp.exp( -x)/x
F = sp.integrate( f, (x, -1, 10))
print( F)
integral = sp.Integral( f, (x, -1, 10))
print( float( integral.evalf()))
