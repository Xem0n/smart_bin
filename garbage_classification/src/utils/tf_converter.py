import os
import argparse
import tensorflow as tf
from tensorflow import keras

def get_model(model_path):
    if not os.path.exists(model_path):
        raise FileNotFoundError(f'{model_path} does not exist.')

    return keras.models.load_model(model_path, compile=False)

def convert_keras_to_lite(keras_path, lite_path):
    model = get_model(keras_path)
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    tflite_model = converter.convert()

    with open(lite_path, 'wb') as file:
        file.write(tflite_model)

    print('Model converted successfully.')

def main():
    parser = argparse.ArgumentParser(
        prog='Keras to TensorFlow Lite Converter',
        description='Convert a keras model to a TensorFlow Lite one.',
    )
    parser.add_argument(
        '-s',
        '--source',
        help='Directory of keras model.',
        required=True,
    )
    parser.add_argument(
        '-t',
        '--target',
        help='Where to save the TensorFlow Lite model.',
        required=True,
    )

    args = parser.parse_args()

    convert_keras_to_lite(args.source, args.target)

if __name__ == '__main__':
    main()
