import sympy as sp
from sympy.parsing.sympy_parser import parse_expr

# Eingabeaufforderung
print("Kreisgleichung eingeben (Bsp.: x**2 + y**2 = 9):")
KG = input("Kreisgleichung: ")

print("Geradengleichung eingeben (Bsp.: 3*x + 4*y = 8):")
GG = input("Geradengleichung: ")

# Definition der Symbole
x, y = sp.symbols('x y')

# Eingaben in linke und rechte Seite aufteilen
KG_links, KG_rechts = map(parse_expr, KG.split('='))
GG_links, GG_rechts = map(parse_expr, GG.split('='))
    
# Gleichungen konstruieren
Kreis = sp.Eq(KG_links, KG_rechts)
Gerade = sp.Eq(GG_links, GG_rechts)

# Schnittpunkte berechnen
Schnittpunkte = sp.solve([Kreis, Gerade], (x, y))

# Ergebnisse ausgeben
print("Die Schnittpunkte sind:", Schnittpunkte)
