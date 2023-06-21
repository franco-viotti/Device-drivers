from flask import Flask, render_template
import os

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/sensar_boton')
def sensar_boton():
    os.system('echo 0 > /dev/my_gpio_driver')
    return 'Señal del botón sensada.'

@app.route('/sensar_microfono')
def sensar_microfono():
    os.system('echo 1 > /dev/my_gpio_driver')
    return 'Señal del micrófono sensada.'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, debug=True)
