import os

# Abrir el archivo de dispositivo en modo de lectura y escritura
dev = os.open('/dev/mychardev', os.O_RDWR)

# Leer los datos del archivo de dispositivo
data = os.read(dev, 1024)

# Imprimir los datos leídos
print(data)

# Cerrar el archivo de dispositivo
os.close(dev)
