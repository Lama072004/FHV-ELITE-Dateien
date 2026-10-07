# -*- coding: utf-8 -*-
"""
Python-Projekt
Christian Heindl
Caesar
"""
from funktionen import caesar
from funktionen import caesar_haeufigkeit

"""
Caesar-Verschlüsselung
"""

alphabet="abcdefghijklmnopqrstuvwxyz"
ALPHABET="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
legale_sonderzeichen=" ,.:;'-?!ßäöüÄÖÜ"


while 1:
    
    nachricht=input("zu verschlüsselne Nachricht eingeben (Abbruch mit %): ")
    if nachricht=="%":
        print("Abbruch")
        break
    counter=0
    for i in range(0,len(nachricht)):  #prüfe ob alle eingegbenen Zeichen ok sind
        if nachricht[i] not in legale_sonderzeichen+alphabet+ALPHABET:            
            counter+=1
    if counter>0:
        print("Eingabe ungültig")
        continue

    anzahl_verschiebungen=input("Anzahl der Verschiebungen eingeben (Ganzzahl): ")  #funktioniert für alle Ganzzahlen
    try:
        anzahl_verschiebungen=int(anzahl_verschiebungen)
    except:
        print("Eingabe falsch, Ganzzahl wurde erwartet")
        continue
   
    verschluesselung=caesar(nachricht, anzahl_verschiebungen)
    
    print(verschluesselung)
    
    
    
"""
Caesar-Entschlüsselung
"""

"""
bei Kenntnis der Anzahl der Verschiebungen
"""
anzahl_verschiebungen=7
nachricht=caesar("hallo",anzahl_verschiebungen)

klartext=caesar(nachricht, -anzahl_verschiebungen)
print("klartext",klartext)

"""
Häufigkeitsanalyse
"""
code=caesar("kleineseinmaleins",5) #häufigster Buchstabe im Klartext ist e

vermuteter_klartext=caesar_haeufigkeit(code)    

print("vermuteter Klartext:",vermuteter_klartext)


"""
brute-force-Methode
"""
code=caesar("kleineseinmaleins",5)
kurzcode=code[0:21]  #die ersten 21 Zeichen sollten zur Entscheidung reichen
for i in range(0,len(alphabet)):
    print(f"text nach {i} Verschiebungen:",caesar(kurzcode,-i))

#manuell anzahl_verschiebungen ändern
anzahl_verschiebungen=-5 #wichtig ist das minus (oder 21)

print(caesar(code, anzahl_verschiebungen))