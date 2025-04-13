plaintext = "Hallo Welt, wie geht es dir".encode()  # Klartext in Bytes
block_size = 8

# Padding hinzufügen
padding_length = block_size - (len(plaintext) % block_size)
padded_text = plaintext + bytes([padding_length] * padding_length)

print("Padded Text:", padded_text)