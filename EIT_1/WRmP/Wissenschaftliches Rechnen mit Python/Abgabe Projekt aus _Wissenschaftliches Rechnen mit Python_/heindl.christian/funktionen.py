# -*- coding: utf-8 -*-
"""
Python-Projekt
Christian Heindl
benoetigte Funktionen
"""

import numpy as np
from collections import Counter
import math as m

alphabet="abcdefghijklmnopqrstuvwxyz"
ALPHABET="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
legale_sonderzeichen=" ,.:;'-?!ßäöüÄÖÜ"
legale_sonderzeichen_2=" ,.:;'-?!"

#Vigenere-Quadrat
quadrat=np.empty((26,26), dtype=np.dtypes.StringDType) #damit man strings in numpy-Matrix schreiben kann
for j in range(0,len(alphabet)):
    quadrat[0][j]=alphabet[j]

for i in range(1,len(alphabet)):
    newline=alphabet[i:len(alphabet)]+alphabet[0:i]
    for j in range(0,len(newline)):
        quadrat[i][j]=newline[j]


def caesar(nachricht,anzahl_verschiebungen):
    """
    Verschlüsselt Nachricht mit der Caesar-Methode

    Parameters
    ----------
    nachricht : string
        zu verschlüsselne Nachricht
    anzahl_verschiebungen : int
        gibt an um wie viele Stellen die Buchstaben verschoben werden sollen

    Returns
    -------
    result : string
        verschlüsselte Botschaft

    """
    antwort=[]
    "Umlaute und ß"
    nachricht=nachricht.replace('ß', 'ss')
    nachricht=nachricht.replace('ä', 'ae')
    nachricht=nachricht.replace('ö', 'oe')
    nachricht=nachricht.replace('ü', 'ue')
    nachricht=nachricht.replace('Ä', 'AE')
    nachricht=nachricht.replace('Ö', 'OE')
    nachricht=nachricht.replace('Ü', 'UE')
    
    "Groß- zu Kleinbuchstaben"
    for i in range(0,len(ALPHABET)):
        nachricht=nachricht.replace(ALPHABET[i], alphabet[i])
        
        
    for sign in nachricht:
        if sign in legale_sonderzeichen: #legale Sonderzeichen bleiben unverändert
            antwort.append(sign)
        else:
            original_index=alphabet.index(sign)
            
            new_index=original_index+anzahl_verschiebungen  #Position des neuen Buchstabens
            new_index=new_index%len(alphabet)    #damit bei alphabet wieder von vorne begonnen wird   
            newsign=alphabet[new_index]
            antwort.append(newsign)
    result=""
    for i in range(0, len(antwort)):
        result+=antwort[i]
    return result



def caesar_haeufigkeit(code):
    """
    versucht durch die Annahme, dass e der häufigste Buchstabe im Klartext
    ist den Code zu dechiffrieren

    Parameters
    ----------
    code : string
        mit Caesar verschlüsselte Nachricht

    Returns
    -------
    string
        vermuteter Klartext

    """
    hb=haeufigster_buchstabe(code)
    
    #suche hb in alphabet
    index_hb=alphabet.index(hb)
    
    index_e=4 #index von e in alphabet
    
    vermutete_verschiebungen=(index_hb-index_e)%len(alphabet)
    
    return caesar(code,-vermutete_verschiebungen)



def haeufigster_buchstabe(text):
    """
    findet (einen) der am häufigsten Buchstaben (keine Sonderzeichen) in einem String

    Parameters
    ----------
    text : string
        string der nach am häufigsten auftretenden Buchstaben analysiert werden soll

    Returns
    -------
    maxletter : string
        einer der am häufigsten auftretenden Buchstaben in text

    """
    #überprüfen ob text ein string ist

    if(type(text)!=str):
        print("Input ist kein String")
        return None
    #entferne Leerzeichen etc
    for i in legale_sonderzeichen:
        text=text.replace(i,"")
    
    
    text=list(text)
    maximum=0
    maxletter=""
    for letter in alphabet:
        if maximum<text.count(letter):
            maximum=text.count(letter)
            maxletter=letter
    return maxletter



def vigenere(nachricht, schluessel):
    """
    Vigenere-Verschlüsselung einer Nachricht

    Parameters
    ----------
    nachricht : string
        zu verschlüsselner Klartext
    schluessel : string
        Schlüssel für Vigenere-Verschlüsselung

    Returns
    -------
    result : string
        mit Vigenere verschlüsselter Code

    """
    antwort=[]
    "Umlaute und ß"
    nachricht=nachricht.replace('ß', 'ss')
    nachricht=nachricht.replace('ä', 'ae')
    nachricht=nachricht.replace('ö', 'oe')
    nachricht=nachricht.replace('ü', 'ue')
    nachricht=nachricht.replace('Ä', 'AE')
    nachricht=nachricht.replace('Ö', 'OE')
    nachricht=nachricht.replace('Ü', 'UE')
    
    for i in legale_sonderzeichen_2:
        nachricht=nachricht.replace(i,"")
        schluessel=schluessel.replace(i,"")
    
    
    "Groß- zu Kleinbuchstaben"
    for i in range(0,len(ALPHABET)):
        nachricht=nachricht.replace(ALPHABET[i], alphabet[i])
        schluessel=schluessel.replace(ALPHABET[i], alphabet[i])
        
    for i in range(0,len(nachricht)):
            index_sign=alphabet.index(nachricht[i])
            index_schluessel=alphabet.index(schluessel[i%len(schluessel)])
            newsign=quadrat[index_sign][index_schluessel]
            antwort.append(newsign)
    result=""
    for i in range(0, len(antwort)):
        if i%5==4:  #Anordnung in 5-er Blöcken
            result+=antwort[i]+" "
        else:
            result+=antwort[i]
    return result

def vigenere_mit_schluessel_entschluesseln(code, schluessel):
    """
    entschlüsselt eine mit Vigenere verschlüsselte Nachricht falls 
    Schlüssel bekannt

    Parameters
    ----------
    code : string
        verschlüsselte Nachricht
    schluessel : string
        Schlüssel mit dem verschlüsselt wurde

    Returns
    -------
    result : string
        entschlüsselter Klartext

    """
    antwort=[]

    
    "Groß- zu Kleinbuchstaben"
    for i in range(0,len(ALPHABET)):
        code=code.replace(ALPHABET[i], alphabet[i])
        schluessel=schluessel.replace(ALPHABET[i], alphabet[i])
    for i in legale_sonderzeichen_2:
        code=code.replace(i,"")
        schluessel=schluessel.replace(i,"")
    
    for i in range(0,len(code)):
        index_schluessel=alphabet.index(schluessel[i%len(schluessel)])
        #index_newsign=np.where(quadrat[index_schluessel]==code[i])[0][0]
        index_newsign=0
        while quadrat[index_schluessel][index_newsign]!=code[i]:
            index_newsign+=1
        newsign=alphabet[index_newsign]
        antwort.append(newsign)
    result=""
    for i in range(0, len(antwort)):
        result+=antwort[i]
    return result

def kasiski(code):
    """
    versucht den Schlüssel einer mit Vigenere verschlüsselten Nachricht
    zu ermitteln
    KEINE ERFOLGSGARANTIE

    Parameters
    ----------
    code : string
        mit Vigenere verschlüsselter Code

    Returns
    -------
    schluessel : string
        mutmasslicher Schlüssel, mit dem code verschlüsselt wurde

    """
    schluessel=""
    result=[]
    for i in legale_sonderzeichen_2:
        code=code.replace(i,"")
    laenge=schluessel_laenge(code)
    for i in range(0,laenge):
        caesar_liste=[]
        for j in range(0, m.floor(len(code)/laenge)):
            caesar_liste.append(code[i+j*laenge])
        hb=most_frequent(caesar_liste)[0]
        index_hb=alphabet.index(hb)
        index_e=4 #index von e in alphabet
        
        vermutete_verschiebungen=(index_hb-index_e)%len(alphabet)
        result.append(alphabet[vermutete_verschiebungen])
    for i in range(0, len(result)):
        schluessel+=result[i]
    return schluessel


def schluessel_laenge(code):
    """
    versucht die Länge des Schlüssels mit dem eine Vigenere verschlüsselte
    Nachricht verschlüsselt wurde zu ermitteln

    Parameters
    ----------
    code : string
        mit Vigenere verschlüsselte Nachricht

    Returns
    -------
    answer : int
        vermutete Schlüssellänge

    """
    for i in legale_sonderzeichen_2:
        code=code.replace(i,"")
    notfound=True
    blocklen=m.floor(len(code)/3)
    block=""
    
    counter=0
    while notfound and blocklen>0:
        listofblocks=[]
        for j in range(0,len(code)-blocklen+1):
            listofblocks.append(code[j:j+blocklen])
        
        counter=most_frequent(listofblocks)[1]
        block=most_frequent(listofblocks)[0]
        if(counter>2):
            notfound=False
        blocklen-=1
    print("Länge der Blöcke: ",blocklen+1)
    list_of_index=[]
    for i in range(0,counter):
        if i==0:
            index=listofblocks.index(block)
            index_teil=index
        else:
            index_teil=listofblocks.index(block)
            index=index_teil+list_of_index[i-1]+1
            
        list_of_index.append(index)
        listofblocks=listofblocks[index_teil+1:len(listofblocks)]
    print("Liste der Indeces: ",list_of_index)
    diff=[]
    for i in range(0,len(list_of_index)-1):
        diff.append(list_of_index[i+1]-list_of_index[i])
    list_ggT=[]
    list_ggT.append(diff[0])
    for i in range(0, len(diff)-1):
        list_ggT.append(ggT(diff[i+1],list_ggT[i]))
    if list_ggT==[]:
        answer=None
    else:
        answer=list_ggT[-1]
    return answer


def most_frequent(liste):
    """
    bestimmt eines der am häufigsten vorkommenden Elemente einer Liste
    und seine Häufigkeit

    Parameters
    ----------
    liste : list
        Liste, die analysiert werden soll

    Returns
    -------
    TYPE
        eines der häufigsten Elemente
    int
        Häufigkeit dieses Elements

    """
    counting=Counter(liste)  #zählt wie oft ein Element in Liste vorkommt
    #wir sind nur am häufigsten Element und seiner Häufigkeit interessiert
    return (counting.most_common(1)[0][0], counting.most_common(1)[0][1])

def ggT(zahl1,zahl2):
    """
    Euklidischer Algorithmus: bestimmt den größten gemeinsamen Teiler
    zweier Ganzzahlen ungleich 0

    Parameters
    ----------
    zahl1 : int
        Ganzzahl ungleich 0
    zahl2 : int
        Ganzzahl ungleich 0

    Returns
    -------
    int oder None
        ggT von zahl1 und zahl2 oder None

    """
    if zahl1==0 or zahl2==0:
        return None
    rest=1
    if zahl1>zahl2:
        a=zahl1
        b=zahl2
    else:
        a=zahl2
        b=zahl1
    while rest!=0:
        rest=a%b
        a=b
        b=rest
    return abs(a)