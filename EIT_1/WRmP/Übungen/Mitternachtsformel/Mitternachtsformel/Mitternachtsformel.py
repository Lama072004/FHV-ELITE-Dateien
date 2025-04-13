def sqrfnc(x, *, a=0.25, b=-2, c=-4):
    return a*(x**2) + b*x + c

def findNullstelle(a, b, c):
    #Berechnung der Diskriminante
    d = b**2 - 4*a*c
    print("d")
