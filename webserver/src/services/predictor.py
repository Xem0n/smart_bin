import os
from tensorflow import keras

def load_model():
    model_path = os.getenv('MODEL_PATH')
    model = keras.models.load_model(model_path)
    probability_model = keras.Sequential([
        model,
        keras.layers.Softmax()
    ])

    return probability_model

model = load_model()


def predict(image):
    return model.predict(image)[0]
