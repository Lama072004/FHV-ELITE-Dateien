import sympy as sp

x = sp.Symbol("x", real =True)
f= -2*x**2+4*x
print (f)
print(type(f))

df = f.diff(x)
print(df)
x_ew = sp.solve(df, x)
print(x_ew)

if f.diff(x).diff(x)>0:
    print("Extremum ist Minimum")
else:
    print("Extremum ist Maximum")

from sympy.plotting import plot

pf = plot(f, df, xlim=(-1, 5), ylim=(-3, 3))