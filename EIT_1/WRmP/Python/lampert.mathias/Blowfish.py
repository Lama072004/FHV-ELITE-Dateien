from Crypto.Cipher import Blowfish
from Crypto.Util.Padding import pad, unpad
import base64

def blowfish_encrypt(key, plaintext):
    cipher = Blowfish.new(key, Blowfish.MODE_ECB)
    padded_text = pad(plaintext.encode(), Blowfish.block_size)
    encrypted_text = cipher.encrypt(padded_text)
    encoded_encrypted_text = base64.b64encode(encrypted_text).decode()
    return encoded_encrypted_text

plaintext = input("Gib den zu verschlüsselnden Text ein: ")
key = input("Gib den Verschlüsselungskey ein: ").encode()  # Konvertiere den Schlüssel in Bytes

encrypted_text = blowfish_encrypt(key, plaintext)
print(f"Encrypted Text: {encrypted_text}")
print(f"Decrypted Text: {plaintext}")