def transposition_decrypt(encrypted_text, key):
    # Berechne die Anzahl der Spalten und Zeilen
    num_columns = int(key)
    num_rows = len(encrypted_text) // num_columns
    num_short_columns = len(encrypted_text) % num_columns

    # Erstelle eine Matrix mit leeren Strings
    matrix = [''] * num_columns
    current_pos = 0

    # Fülle die Matrix mit den verschlüsselten Zeichen
    for col in range(num_columns):
        col_length = num_rows + (1 if col < num_short_columns else 0)
        matrix[col] = encrypted_text[current_pos:current_pos + col_length]
        current_pos += col_length

    # Lese die Matrix zeilenweise aus, um den ursprünglichen Text zu rekonstruieren
    decrypted_text = ''
    for row in range(num_rows):
        for col in range(num_columns):
            if row < len(matrix[col]):
                decrypted_text += matrix[col][row]

    return decrypted_text

encrypted_text = input("Gib den verschlüsselten Text ein: ")
key = input("Gib den Verschlüsselungskey ein: ")

decrypted_text = transposition_decrypt(encrypted_text, key)

print(f"Verschlüsselter Text: {encrypted_text}")
print(f"Entschlüsselter Text: {decrypted_text}")
