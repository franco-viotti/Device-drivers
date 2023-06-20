import serial

# Configurar la comunicación serial
puerto = '/dev/ttyUSB0'  # Cambiar si es necesario
velocidad = 9600  # Cambiar según la velocidad de comunicación del Arduino

arduino = serial.Serial(puerto, velocidad)

# Leer los mensajes del Arduino
while True:
    mensaje = arduino.readline().decode().strip()
    print("Mensaje recibido: ", mensaje)

arduino.close()

