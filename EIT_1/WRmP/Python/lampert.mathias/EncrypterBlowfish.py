from Crypto.Cipher import Blowfish
from Crypto.Util.Padding import pad, unpad
import base64



def blowfish_decrypt(key, encrypted_text):
    cipher = Blowfish.new(key, Blowfish.MODE_ECB)
    decoded_encrypted_text = base64.b64decode(encrypted_text)
    decrypted_padded_text = cipher.decrypt(decoded_encrypted_text)
    decrypted_text = unpad(decrypted_padded_text, Blowfish.block_size).decode()
    return decrypted_text

# Entschlüsselung:
encrypted_text = input("Gib den verschlüsselten Text ein: ")
key = input("Gib den Verschlüsselungskey ein: ").encode()  # Konvertiere den Schlüssel in Bytes

decrypted_text = blowfish_decrypt(key, encrypted_text)
print(f"Entschlüsselter Text: {decrypted_text}")