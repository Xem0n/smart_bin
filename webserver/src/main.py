from dotenv import load_dotenv
load_dotenv()

import uuid
import numpy as np
from flask import Flask, request
from services.predictor import predict
from db.database import init_db, db_session
from services.database import init_bin, add_garbage, get_bins_data
from services.utils import unify_bin_data, transform_image

app = Flask(__name__)
init_db()

@app.teardown_appcontext
def shutdown_session(exception=None):
    db_session.remove()

@app.get('/')
def get_data():
    (bins, categories, garbages) = get_bins_data()
    return unify_bin_data(bins, categories, garbages)

@app.post('/')
def predict_image():
    print('start...')
    data = request.get_data()
    id = uuid.uuid4()

    print(id)

    with open(f'tmp/{id}.jpg', 'wb') as f:
        f.write(data)

    print('transform')
    image = transform_image(id)

    print('predict')
    prediction = predict(image)

    print(prediction)
    result = np.argmax(prediction)

    add_garbage(request.headers.get('Mac-Address', ''), result)

    # result must be one bigger
    # bcuz strtol() in C fails to 0 value
    # which unables to check whether body is invalid or of type equal to 0
    return str(result + 1)

    # return '0'

if __name__ == '__main__':
    # make sure to allow port in firewall
    app.run(host="0.0.0.0", port=5000, debug=True)
