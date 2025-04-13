import sympy as sp

x, y = sp.symbols("x y")
rr = sp.solve(x +2*(y**2)*2*y+2*x, y)  # Solve the equation x + 2*y = 0 for y
r = rr
print(r)
