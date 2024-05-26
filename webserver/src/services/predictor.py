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

def get_size():
    return model.layers[0].input_shape[1:3][::-1]

def get_color_mode():
    return 'RGB' if model.layers[0].input_shape[3] == 3 else 'L'

def predict(image):
    return model.predict(image)[0]
