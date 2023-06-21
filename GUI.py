import os

while True:
    opcion = input('¿Qué señal quieres Sensar? Ingresa "P" para pulsado, "S" para sensor, o "exit" para salir: ')

    if opcion == 'P':
        os.system('echo 0 > /dev/my_gpio_driver')

    elif opcion == 'S':
        os.system('echo 1 > /dev/my_gpio_driver')

    elif opcion == 'exit':
        break

    else:
        print('Opción inválida. Por favor, ingresa "P", "S" o "exit".')

print('Programa terminado.')
