from tensorflow import keras
from keras import layers
from keras.preprocessing import image
from matplotlib import pyplot as plt
from dotenv import load_dotenv
import numpy as np
import random
import os
import argparse

load_dotenv()

parser = argparse.ArgumentParser(
    prog='Garbace Classification Test',
    description='Test random image from dataset.',
)
parser.add_argument(
    '-t',
    '--type',
    default='metal',
    help='Type of garbage to test.',
    required=True,
    choices=['metal', 'paper', 'plastic'],
)

args = parser.parse_args()

model = keras.models.load_model(os.getenv('MODEL_PATH'))
probability_model = keras.Sequential([
    model,
    keras.layers.Softmax()
])

img_width = 512
img_height = 384
class_names = ['metal', 'paper', 'plastic']

dataset_path = os.getenv('DATASET_PATH')
sdir = f'{dataset_path}/{args.type}/'
flist = os.listdir(sdir)
test_img = random.choice(flist)
test_img = os.path.join(sdir, test_img)

img = image.load_img(test_img, target_size=(img_height, img_width))
img = image.img_to_array(img, dtype=np.uint8)

predictions_single = probability_model.predict(np.expand_dims(img, 0))

def plot_value_array(i, predictions_array):
    plt.grid(False)
    plt.xticks(range(3))
    plt.yticks([])
    thisplot = plt.bar(range(3), predictions_array, color="#777777")
    plt.ylim([0, 1])

fig = plt.figure()

fig.add_subplot(121)
plt.axis('off')
plt.imshow(img, cmap=plt.cm.binary)

fig.add_subplot(122)
plot_value_array(1, predictions_single[0])
_ = plt.xticks(range(3), class_names, rotation=45)

plt.show()
