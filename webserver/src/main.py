from dotenv import load_dotenv
load_dotenv()

import numpy as np
from datetime import datetime
from flask import Flask, request
from services.predictor import predict
from db.database import init_db, db_session
from services.database import init_bin, add_garbage, get_bins_data, get_bin_data, update_bin
from services.utils import get_bin_data_dto, transform_image

app = Flask(__name__)
init_db()

@app.teardown_appcontext
def shutdown_session(exception=None):
    db_session.remove()

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

        return bin.color
    except:
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
    print('start...')
    data = request.get_data()
    print('data received')
    id = str(datetime.now())

    print(id)

    with open(f'tmp/{id}.jpg', 'wb') as f:
        f.write(data)

    try:
        print('transform')
        image = transform_image(id, data)

        print('predict')
        prediction = predict(image)

        print(prediction)
        result = np.argmax(prediction)

        add_garbage(request.headers.get('Mac-Address', ''), result)

        # result must be one bigger
        # bcuz strtol() in C fails to 0 value
        # which unables to check whether body is invalid or of type equal to 0
        return str(result + 1)
    except Exception as e:
        print(e)
        return '0'

if __name__ == '__main__':
    # make sure to allow port in firewall
    app.run(host="0.0.0.0", port=5000, debug=True)
