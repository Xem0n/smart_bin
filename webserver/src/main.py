import uuid
import numpy as np
from flask import Flask, request
from services.predictor import predict, convert_bytes_to_image_batch
from db.database import init_db, db_session
from services.database import init_bin, add_garbage, get_bins_data
from services.utils import unify_bin_data

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
    path = f'tmp/{id}.jpg'
    print(path)

    with open(path, 'wb') as f:
        print('save')
        f.write(data)

    print('convert and predict')
    image = convert_bytes_to_image_batch(data)
    prediction = predict(image)

    print(prediction)
    result = np.argmax(prediction)

    # result must be one bigger
    # bcuz strtol() in C fails to 0 value
    # which unables to check whether body is invalid or of type equal to 0
    return str(result + 1)

if __name__ == '__main__':
    # make sure to allow port in firewall
    app.run(host="0.0.0.0", port=5000, debug=True)
