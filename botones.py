from flask import Flask, render_template
import os

botones = Flask(__name__)

@botones.route('/')
def index():
    return render_template('index.html')

@botones.route('/sensar_boton')
def sensar_boton():
    os.system('echo 0 > /dev/my_gpio_driver')
    return 'Señal del botón sensada.'

@botones.route('/sensar_microfono')
def sensar_microfono():
    os.system('echo 1 > /dev/my_gpio_driver')
    return 'Señal del micrófono sensada.'

if __name__ == '__main__':
    botones.run(host='0.0.0.0', port=80, debug=True)
