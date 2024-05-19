import uuid
import numpy as np
from flask import Flask, request
from predictor import predict, convert_bytes_to_image_batch
from resizer import resize
from db.database import init_db, db_session

app = Flask(__name__)
init_db()

@app.teardown_appcontext
def shutdown_session(exception=None):
    db_session.remove()

@app.post('/')
def predict_image():
    data = request.get_data()
    image = convert_bytes_to_image_batch(data)
    prediction = predict(image)
    result = np.argmax(prediction)

    # result must be one bigger
    # bcuz strtol() in C fails to 0 value
    # which unables to check whether body is invalid or of type equal to 0
    return str(result + 1)

if __name__ == '__main__':
    # make sure to allow port in firewall
    app.run(host="0.0.0.0", port=5000, debug=True)
