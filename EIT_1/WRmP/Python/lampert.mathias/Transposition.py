def transposition_encrypt(text, key):
    # Erstelle eine Liste mit leeren Zeichenketten, die so viele Elemente wie der Schlüssel hat.
    matrix = [''] * int(key)

    # Durchlaufe jede Spalte (jede Position im Schlüssel).
    for column in range(int(key)):
        startp = column  # Setze den Startpunkt auf die aktuelle Spalte.

        # Füge Zeichen aus dem Text zur aktuellen Spalte hinzu, bis das Ende des Textes erreicht ist.
        while startp < len(text):
            matrix[column] += text[startp]  # Füge das Zeichen zur Spalte hinzu.
            startp += int(key)

    # Kombiniere alle Spalten (Zeichenketten in der Matrix) zu einem verschlüsselten Text.
    encrypted_text = ''.join(matrix)
    return encrypted_text

text = input("Gib den zu verschlüsselnden Text ein: ")
key = input("Gib den Verschlüsselungskey ein: ")
encrypted_text = transposition_encrypt(text, key)

print(f"Original Text: {text}")
print(f"Encrypted Text: {encrypted_text}")