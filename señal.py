import serial
import matplotlib.pyplot as plt

# Configuración de la comunicación serial
arduino_port = '/dev/ttyACM0'  # Puerto serial de la Arduino
baud_rate = 9600  # Velocidad de baudios
ser = serial.Serial(arduino_port, baud_rate)

# Configuración del gráfico
plt.ion()  # Modo interactivo de Matplotlib
fig, ax = plt.subplots()
line, = ax.plot([], [], 'b-')  # Línea de datos en color azul
ax.set_xlabel('Tiempo')
ax.set_ylabel('Valor')
ax.set_title('Graficando señal en función del tiempo')
x_data, y_data = [], []

# Bucle principal
while True:
    # Leer el comando del usuario (seleccionar señal)
    command = input("Seleccione la señal a leer (1: humedad, 2: temperatura, 0: salir): ")
    if command == '0':
        break

    # Escribir el comando en el dispositivo
    ser.write(command.encode())

    # Limpiar los datos anteriores
    x_data.clear()
    y_data.clear()

    # Leer la señal desde la comunicación serial durante 1 segundo
    start_time = time.time()
    while time.time() - start_time < 1:
        try:
            # Leer un dato de la comunicación serial
            data = ser.readline().decode().strip()
            if data:
                # Separar los valores de tiempo y señal
                timestamp, value = data.split(',')
                timestamp = float(timestamp)
                value = float(value)

                # Agregar los valores a los datos
                x_data.append(timestamp)
                y_data.append(value)

        except (ValueError, UnicodeDecodeError):
            pass

    # Actualizar el gráfico
    line.set_data(x_data, y_data)
    ax.relim()
    ax.autoscale_view()

    # Mostrar el gráfico
    plt.draw()
    plt.pause(0.01)

# Cerrar la comunicación serial
ser.close()
