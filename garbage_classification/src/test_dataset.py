import os
import argparse
import tensorflow
import numpy as np
from dotenv import load_dotenv
from tensorflow import keras

load_dotenv()

garbageMapping = {
    'metal': 0,
    'paper': 1,
    'plastic': 2
}

def test_dataset(dataset_path, model_path):
    model = keras.models.load_model(model_path)
    probability_model = keras.Sequential([model, keras.layers.Softmax()])

    height = model.layers[0].input_shape[1]
    width = model.layers[0].input_shape[2]
    color_mode = 'rgb' if model.layers[0].input_shape[3] == 3 else 'grayscale'

    amount = 0
    successful = 0

    for root, dirs, files in os.walk(dataset_path):
        for file in files:
            amount += 1
            img_path = os.path.join(root, file)
            success = test_image(img_path, model, height, width, color_mode)
            successful += 1 if success else 0

    return successful / amount

def test_image(img_path, model, height, width, color_mode):
    category = img_path.split(os.sep)[-2]
    img = keras.preprocessing.image.load_img(img_path, target_size=(height, width), color_mode=color_mode)
    img = keras.preprocessing.image.img_to_array(img, dtype=np.uint8)
    img = np.expand_dims(img, 0)

    predictions = model.predict(img)[0]

    return garbageMapping[category] == np.argmax(predictions)

def main():
    parser = argparse.ArgumentParser(
        prog='Garbace Classification Test',
        description='Test dataset and return success percentage.',
    )
    parser.add_argument(
        '-d',
        '--dataset',
        help='Path to the dataset.',
    )
    parser.add_argument(
        '-m',
        '--model',
        help='Path to the model. If not provided, a model provided in the environment variable MODEL_PATH will be used.',
    )

    args = parser.parse_args()
    model_path = args.model or os.getenv('MODEL_PATH')

    success_ratio = test_dataset(args.dataset, model_path)
    print('Success ratio: {:5.2f}%'.format(success_ratio * 100))


if __name__ == '__main__':
    main()
