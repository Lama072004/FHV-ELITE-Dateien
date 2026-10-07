# -*- coding: utf-8 -*-
"""
Python-Projekt
Christian Heindl
Laufzeitanalysen
"""

from time import time
import matplotlib.pyplot as plt
from funktionen import caesar
from funktionen import haeufigster_buchstabe
from funktionen import caesar_haeufigkeit
from funktionen import vigenere
from funktionen import vigenere_mit_schluessel_entschluesseln
from funktionen import kasiski
from dieglocke import text as glocke


"""
Caesar-Verschlüsselung
"""

"""
dauer=[]
for n in range(1,100):
    message=100000*n*"a"
    t1=time()
    caesar(message, 10) #die 10 ist willkürlich gewählt und hat keinen 
                        #Einfluss auf das Ergebnis
    t2=time()
    dauer.append(t2-t1)
    
    
#plotte das Ergebnis
x=range(1,100)


pltcaesar=plt.figure(figsize=(4, 3))
plt.plot(x, dauer, linestyle='-', color='red')


plt.xlabel('Länge der Nachricht durch 100000')
plt.ylabel("Laufzeit Caesar-Chiffre in s")
plt.grid(True)
plt.show()

#pltcaesar.savefig("Laufzeit Caesar-Chiffre.pdf", format="pdf", dpi=300, transparent=True, bbox_inches="tight")
"""


"""
Caesar-Entschlüsselung
"""
#mit Kenntnis der Anzahl der Verschiebungen
#keine Laufzeitanalyse notwendig, da nur wieder Caesar-Chiffre angewendet wird

#Häufigkeitsanalyse
"""
dauer=[]
for n in range(1,100):
    message=1000*n*"kleineseinmaleins"
    t1=time()
    caesar_haeufigkeit(message)  
    t2=time()
    dauer.append(t2-t1)
    
    
#plotte das Ergebnis
x=range(1,100)


pltcaesarhaeufig=plt.figure(figsize=(4, 3))
plt.plot(x, dauer, linestyle='-', color='blue')


plt.xlabel('Länge der Nachricht durch 17000')
plt.ylabel("Laufzeit Caesar-Entschlüsselung\n mit Häufigkeit in s")
plt.grid(True)
#plt.legend(False)
plt.show()

pltcaesarhaeufig.savefig("Laufzeit Caesar-Entschlüsselung-Häufigkeit.pdf", format="pdf", dpi=300, transparent=True, bbox_inches="tight")
"""

#Brute-force
#eine Laufzeitanalyse für brute-force erübrigt sich, da man nur einen relativ kurzen
#Teil der codierten Nachricht verwendet um zu entscheiden welche Verschiebung
#die richtige ist. Danach wendet man einmal den Caesar-Chiffre auf den gesamten
#Text an. Die Laufzeit des Caesar-Chriffres wurde bereits analysiert



"""
Vigenere-Verschlüsselung
"""

"""
dauer=[]
for n in range(1,100):
    message=100000*n*"a"
    schluessel="dasisteinschluessel"
    t1=time()
    vigenere(message, schluessel) 
    t2=time()
    dauer.append(t2-t1)
    
    
#plotte das Ergebnis
x=range(1,100)


pltvige=plt.figure(figsize=(4, 3))
plt.plot(x, dauer, linestyle='-', color='red')


plt.xlabel('Länge der Nachricht durch 100000')
plt.ylabel("Laufzeit Vigenere-Chiffre in s")
plt.grid(True)
plt.show()

#pltvige.savefig("Laufzeit Vigenere-Chiffre.pdf", format="pdf", dpi=300, transparent=True, bbox_inches="tight")
"""


"""
Vigenere-Entschlüsselung
"""
#mit Schlüssel


"""
dauer=[]
for n in range(1,100):
    message=100000*n*"a"
    schluessel="dasisteinschluessel"
    message=vigenere(message, schluessel)
    t1=time()
    vigenere_mit_schluessel_entschluesseln(message, schluessel) 
    t2=time()
    dauer.append(t2-t1)
    
    
#plotte das Ergebnis
x=range(1,100)


pltvigemitschluessel=plt.figure(figsize=(4, 3))
plt.plot(x, dauer, linestyle='-', color='blue')


plt.xlabel('Länge der Nachricht durch 100000')
plt.ylabel("Laufzeit Vigenere-Entschlüsselung\n mit Schlüssel in s")
plt.grid(True)
plt.show()

#pltvigemitschluessel.savefig("Laufzeit Vigenere-Entschlüsselung mit Schlüssel.pdf", format="pdf", dpi=300, transparent=True, bbox_inches="tight")
"""

#Kasiski-Methode

"""
dauer=[]
for n in range(1,100):
    message=glocke[0:n*100]
    schluessel="schiller"
    message=vigenere(message, schluessel)
    print(message)
    t1=time()
    kasiski(message)
    t2=time()
    dauer.append(t2-t1)
    
    
#plotte das Ergebnis
x=range(1,100)


pltvigekasiski=plt.figure(figsize=(4, 3))
plt.plot(x, dauer, linestyle='-', color='green')


plt.xlabel('Länge der Nachricht durch 100')
plt.ylabel("Laufzeit Vigenere-Entschlüsselung\n mit Kasiski in s")
plt.grid(True)
plt.show()

#pltvigekasiski.savefig("Laufzeit Vigenere-Entschlüsselung mit Kasiski.pdf", format="pdf", dpi=300, transparent=True, bbox_inches="tight")
"""