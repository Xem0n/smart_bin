import os
import io
import numpy as np
from tensorflow import keras
from PIL import Image
from dotenv import load_dotenv

load_dotenv()

def load_model():
    model_path = os.getenv('MODEL_PATH')
    model = keras.models.load_model(model_path)
    probability_model = keras.Sequential([
        model,
        keras.layers.Softmax()
    ])

    return probability_model

model = load_model()

def convert_bytes_to_image_batch(bytes):
    return np.expand_dims(np.array(Image.open(io.BytesIO(bytes))), 0)

def predict(image):
    return model.predict(image)[0]
