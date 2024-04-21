import os
import tensorflow as tf
import keras
from keras import layers
from dotenv import load_dotenv

load_dotenv()

batch_size = int(os.getenv('BATCH_SIZE'))
img_width = int(os.getenv('IMAGE_WIDTH'))
img_height = int(os.getenv('IMAGE_HEIGHT'))
dataset_path = os.getenv('DATASET_PATH')
dataset_path = f'./{dataset_path}/'

train_ds = keras.utils.image_dataset_from_directory(
    dataset_path,
    validation_split=0.2,
    subset='training',
    seed=123,
    image_size=(img_height, img_width),
    batch_size=batch_size,
)
val_ds = keras.utils.image_dataset_from_directory(
    dataset_path,
    validation_split=0.2,
    subset='validation',
    seed=123,
    image_size=(img_height, img_width),
    batch_size=batch_size,
)

class_names = train_ds.class_names
num_classes = len(class_names)

model = keras.Sequential([
    keras.Input(shape=(img_height, img_width, 3)),
    keras.layers.Rescaling(1./255),
    keras.layers.Conv2D(32, 3, activation='relu'),
    keras.layers.MaxPooling2D(),
    keras.layers.Conv2D(32, 3, activation='relu'),
    keras.layers.MaxPooling2D(),
    keras.layers.Conv2D(32, 3, activation='relu'),
    keras.layers.MaxPooling2D(),
    keras.layers.Flatten(),
    keras.layers.Dense(128, activation='relu'),
    keras.layers.Dense(num_classes),
])

model.compile(
  optimizer='adam',
  loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True),
  metrics=['accuracy'],
)

model.fit(
    train_ds,
    validation_data=val_ds,
    epochs=10,
)

model.save(os.getenv('MODEL_PATH'))
