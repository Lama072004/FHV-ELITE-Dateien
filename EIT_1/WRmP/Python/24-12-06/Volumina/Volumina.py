from math import *
import os

# Definition der Funktionen
def vol_kugel(r):
    assert isinstance(r, float)
    return (4/3) * r**3 * pi

def vol_kegel(r, h):
    assert isinstance(r, float)
    assert isinstance(h, float)
    return (1/3) * r**2 * pi * h

def vol_zylinder(r, h):
    assert isinstance(r, float)
    assert isinstance(h, float)
    return r**2 * pi * h

# Wir erstellen eine Datei und schreiben Inhalte hinein
with open("mein_file.txt", "w") as f:
    f.write("kegel::42::13\n")
    f.write("kugel::42\n")
    f.write("zylinder::142::13\n")

# Variante 1: Datei Zeile für Zeile lesen und ausgeben
print("Variante 1:")
with open("mein_file.txt", "r") as f:
    for line in f.readlines():
        print(line.strip())  # `.strip()` entfernt überflüssige Leerzeichen und Zeilenumbrüche
print()

# Variante 2: Gesamten Inhalt der Datei lesen und Parameter analysieren
print("Variante 2:")
with open("mein_file.txt", "r") as f:
    content = f.read()

for line in content.split("\n"):  # Auf Zeilenumbrüche splitten
    if not line.strip():  # Leere Zeilen ignorieren
        continue
    try:
        # Zeile analysieren
        name, *params = line.split("::")
        params = list(map(float, params))  # Parameter in Float umwandeln
        print(f"Diese Zeile definiert einen '{name}' mit den Parametern {params}")
        
        # Volumen berechnen basierend auf der Form
        if name == "kegel":
            print(f"Volumen des {name} = {vol_kegel(*params)}")
        elif name == "kugel":
            print(f"Volumen des {name} = {vol_kugel(*params)}")
        elif name == "zylinder":
            print(f"Volumen des {name} = {vol_zylinder(*params)}")
        else:
            print(f"Unbekannter Typ: {name}")
    except Exception as e:
        print(f"Fehler beim Verarbeiten der Zeile '{line}': {e}")
