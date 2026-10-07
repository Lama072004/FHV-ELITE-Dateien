import sympy as sp

x = sp.symbols('x')

y = ((sp.e**x+sp.e**-x-2)/(1-sp.cos(x)))

yp = sp.diff(y,x)
print(yp)

ypp = sp.diff(yp,x)
print(ypp)

# Ersetze ** durch ^ in den Ausdrücken
y_str = str(y).replace('**', '^')
yp_str = str(yp).replace('**', '^')
ypp_str = str(ypp).replace('**', '^')

# Ausgabe der Ausdrücke
print(f"y = {y_str}")
print(f"y' = {yp_str}")
print(f"y'' = {ypp_str}")