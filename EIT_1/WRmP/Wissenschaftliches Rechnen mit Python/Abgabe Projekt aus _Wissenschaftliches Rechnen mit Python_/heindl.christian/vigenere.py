# -*- coding: utf-8 -*-
"""
Python-Projekt
Christian Heindl
Vigenere
"""
import numpy as np
from dieglocke import text as glocke 
from funktionen import vigenere
from funktionen import vigenere_mit_schluessel_entschluesseln
from funktionen import kasiski



alphabet="abcdefghijklmnopqrstuvwxyz"
ALPHABET="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
legale_sonderzeichen=" ,.;':-?!ßäöüÄÖÜ"
legale_sonderzeichen_2=" ,.:;'-?!"

"""
Vigenere-Quadrat
"""

quadrat=np.empty((26,26), dtype=np.dtypes.StringDType) #damit man strings
                                                        # in numpy-Matrix schreiben kann
for j in range(0,len(alphabet)):
    quadrat[0][j]=alphabet[j]

for i in range(1,len(alphabet)):
    newline=alphabet[i:len(alphabet)]+alphabet[0:i]
    for j in range(0,len(newline)):
        quadrat[i][j]=newline[j]


maerchen="Endlich kamen sie an ein Häuslein, das aus Brot gebaut und \
mit Kuchen gedeckt war, und die Fenster waren aus hellem Zucker. \
Da wollen wir uns satt essen, sagte Hänsel. Ich will vom Dach essen,\
und du Gretel, kannst vom Fenster essen, das ist fein süß. Hänsel\
brach sich ein wenig vom Dach ab und Gretel knusperte an den \
Fensterscheiben. Da rief auf einmal eine feine Stimme aus dem Häuschen"

schluessel_maerchen="grimm"
code_maerchen=vigenere(maerchen, schluessel_maerchen)
print("Vigenere-Verschlüsselung von maerchen:", code_maerchen)


schluessel_glocke="friedrichschiller"
code_glocke=vigenere(glocke, schluessel_glocke)
#print("Vigenere-Verschlüsselung von glocke",code_glocke)




"""
Entschlüsselung
"""

"""
mit Schlüssel
"""

klartext_maerchen=vigenere_mit_schluessel_entschluesseln(code_maerchen, schluessel_maerchen)

print("Entschlüsselung mit Schlüssel von maerchen:", klartext_maerchen)


"""
ohne Schlüssel
"""

#Kasiski-Methode

#Märchen
mutmasslicher_schluessel_maerchen=kasiski(code_maerchen)
print("vermuteter Schlüssel maerchen:", mutmasslicher_schluessel_maerchen)


mutmasslicher_klartext_maerchen=vigenere_mit_schluessel_entschluesseln(\
 code_maerchen, mutmasslicher_schluessel_maerchen)
print("Vigenere-Verschlüsselung von maerchen:", code_maerchen)
print("vermuterer Klartext maerchen", mutmasslicher_klartext_maerchen)   


#Glocke
mutmasslicher_schluessel_glocke=kasiski(code_glocke)
print("vermuteter Schlüssel glocke:", mutmasslicher_schluessel_glocke)


mutmasslicher_klartext_glocke=vigenere_mit_schluessel_entschluesseln(\
  code_glocke, mutmasslicher_schluessel_glocke)
print("vermuterer Klartext glocke", mutmasslicher_klartext_glocke)       