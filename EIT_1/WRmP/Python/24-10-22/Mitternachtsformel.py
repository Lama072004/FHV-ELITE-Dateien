def sqrfnc(a, b, c):
    return a * (x ** 2) + b * x + c

def findNullstelle(a, b, c):
    # Berechnung der Diskriminante
    d = b ** 2 - 4 * a * c
    print(f"Diskriminante: {d}")
    
    if (a == 0):
        print("Error!! \"a\" kann nicht 0 in einer Quadratischen Funktion sein")
    
    elif d > 0:
        print("Die Diskriminante ist größer als 0. Es gibt zwei Nullstellen.")
        x1 = ((-b+(d**0.5))/2*a)
        x2 = ((-b-(d**0.5))/2*a)
        print(f"x1={x1}   x2={x2}")
        
        #Kontrolle
        if (a*(x1**2) + b*x1 + c == 0):
            print("Wert x1 ist korrekt")
        else:
            print("Wert x1 ist nicht korrekt")
        if (a * (x2**2)+b*x2+c == 0):
            print("Wert x2 ist korrekt")
        else:
            print("Wert x2 ist nicht korrekt")
        
    elif d == 0:
        print("Die Diskriminante ist 0. Es gibt eine Nullstelle.")
        x3 = ((-b-(d**0.5))/2*a)
        print(f"x1={x3}")
        
        #Kontrolle2
        if (a*(x3**2)+b*x3+c == 0):
            print("Wert x3 ist korrekt")
        else:
            print("Wert x3 ist nicht korrekt")
    else:
        print("Die Diskriminante ist kleiner als 0. Es gibt keine Nullstellen.")   

# Benutzer nach Eingaben fragen und in Zahlen umwandeln
a = float(input("Gib den Wert für a ein: "))
b = float(input("Gib den Wert für b ein: "))
c = float(input("Gib den Wert für c ein: "))

# Funktion findNullstelle mit den eingegebenen Werten aufrufen
findNullstelle(a, b, c)