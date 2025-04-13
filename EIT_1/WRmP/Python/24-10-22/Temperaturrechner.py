def celsius_to_kelvin(celsius):
    return celsius + 273.15

def celsius_to_fahrenheit(celsius):
    return (celsius * 9/5) + 32

def kelvin_to_fahrenheit(kelvin):
    return (kelvin - 273.15) * 9/5 + 32

def kelvin_to_celsius(kelvin):
    return kelvin - 273.15

def fahrenheit_to_kelvin(fahrenheit):
    return (fahrenheit - 32) * 5/9 + 273.15

def fahrenheit_to_celsius(fahrenheit):
    return (fahrenheit - 32) * 5/9

while True:
    print("\nWähle eine Umrechnung:")
    print("Celsius nach Kelvin........1")
    print("Celsius nach Fahrenheit....2")
    print("Kelvin nach Fahrenheit.....3")
    print("Kelvin nach Celsius........4")
    print("Fahrenheit nach Kelvin.....5")
    print("Fahrenheit nach Celsius....6")
    print("Programmende...............x")
    
    wahl = input("Wähle eine der aufgelisteten Optionen\n")
    
    
    match wahl:
        case "1":
            print("Umrechnung von Celsius nach Kelvin")
            celsius = float(input("Gib die Temperatur in Grad Celsius ein:\n"))
            
            if celsius == -273.15:
                print("Temperatur ist beim absoluten Nullpunkt")
            elif celsius <= -273.15:
                print("Rechnung nicht möglich -> Temperatur ist niedriger als der absolute Nullpunkt")
            else:
                print(f"Ergebnis: {celsius_to_kelvin(celsius)} K")
        case "2":
            print("Celsius nach Fahrenheit")
            celsius = float(input("Gib die Temperatur in Grad Celsius ein:\n"))
            print(f"Ergebnis: {celsius_to_fahrenheit(celsius)} °F")
        case "3":
            print("Kelvin nach Fahrenheit")
            if kelvin < 0:
                print("Umrechnung nicht möglich :)")
            else:
                kelvin = float(input("Gib die Temperatur in Kelvin ein:\n"))
                print(f"Ergebnis: {kelvin_to_fahrenheit(kelvin)} °F")
            
        case "4":
            print("Kelvin nach Celsius")
            if kelvin < 0:
                print("Umrechnung nicht möglich :)")
            else:
                kelvin = float(input("Gib die Temperatur in Kelvin ein:\n"))
                print(f"Ergebnis: {kelvin_to_celsius(kelvin)} °C")
        case "5":
            print("Fahrenheit nach Kelvin")
            fahrenheit = float(input("Gib die Temperatur in Fahrenheit ein:\n"))
            print(f"Ergebnis: {fahrenheit_to_kelvin(fahrenheit)} K")
        case "6":
            print("Fahrenheit nach Celsius")
            fahrenheit = float(input("Gib die Temperatur in Fahrenheit ein:\n"))
            print(f"Ergebnis: {fahrenheit_to_celsius(fahrenheit)} °C")
        case "x" | "X":
            print("Programm wird beendet.")
            break
        case _:
            print("Ungültige Option. Bitte wähle eine der aufgelisteten Optionen.")