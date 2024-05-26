from dotenv import load_dotenv
load_dotenv()

import os
import requests
import numpy as np
from random import randrange
from datetime import datetime
from flask import Flask, request
from services.predictor import predict
from db.database import init_db, db_session
from services.database import init_bin, add_garbage, get_bins_data, get_bin_data, update_bin
from services.utils import get_bin_data_dto, transform_image

webcam_url = os.getenv('IP_WEBCAM_URL')
print(webcam_url)

app = Flask(__name__)
init_db()

@app.teardown_appcontext
def shutdown_session(exception=None):
    db_session.remove()

# to delete
@app.post('/init')
def test_init():
    init_bin('test')
    return 'ok'

@app.get('/')
def get_data():
    bins = get_bins_data()

    return get_bin_data_dto(bins)

@app.get('/bin/<mac_address>')
def get_bin_color(mac_address):
    try:
        bin = get_bin_data(mac_address)

        print(f'bin color: {bin.color}')
        return bin.color
    except:
        print('not found')
        return '#000000', 404

@app.post('/update')
def update_bin_info():
    data = request.get_json()

    if 'id' not in data or 'name' not in data or 'color' not in data:
        return 'invalid', 400

    update_bin(data['id'], data['name'], data['color'])

    return 'ok'

@app.post('/')
def predict_image():
    print('start')

    try:
        data = fetch_image()
        print('data received')

        id = str(datetime.now())
        print(id)

        with open(f'tmp/{id}.jpg', 'wb') as f:
            f.write(data)

        print('transform')
        image = transform_image(id, data)

        print('predict')
        prediction = predict(image)

        print(prediction)
        result = np.argmax(prediction)

        print(f'result: {result}')

        add_garbage(request.headers.get('Mac-Address', ''), result)

        # result must be one bigger
        # bcuz strtol() in C fails to 0 value
        # which unables to check whether body is invalid or of type equal to 0
        return str(result + 1)
    except Exception as e:
        print(e)
        return '0'

def fetch_image():
    requests.get(f'{webcam_url}/enabletorch')
    r = requests.get(f'{webcam_url}/shot.jpg')
    requests.get(f'{webcam_url}/disabletorch')

    return r.content

# to delete
bin_id = 'ec:62:60:8f:e1:d8'

@app.post('/lolxd')
def random_garbage():
    result = randrange(0, 3)

    try:
        add_garbage(bin_id, result)
        print(f'added {result}')

        return str(result), 200
    except Exception as e:
        print('cos poszlo nie tak xd', e)

        return '-1', 500

if __name__ == '__main__':
    # make sure to allow port in firewall
    app.run(host="0.0.0.0", port=5000, debug=True)
