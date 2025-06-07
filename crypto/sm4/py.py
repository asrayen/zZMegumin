import os

random_bytes = os.urandom(5 * 1024 * 1024)
hex_string = random_bytes.hex()
with open('10MB.txt', 'w') as f:
    f.write(hex_string)
