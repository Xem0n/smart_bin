import os
import tensorflow as tf
import keras
import argparse
from keras import layers
from dotenv import load_dotenv

load_dotenv()

def get_dataset(dataset_path, img_width, img_height, batch_size, color_mode):
    train_ds = keras.utils.image_dataset_from_directory(
        dataset_path,
        validation_split=0.2,
        subset='training',
        seed=123,
        image_size=(img_height, img_width),
        batch_size=batch_size,
        color_mode=color_mode,
    )
    val_ds = keras.utils.image_dataset_from_directory(
        dataset_path,
        validation_split=0.2,
        subset='validation',
        seed=123,
        image_size=(img_height, img_width),
        batch_size=batch_size,
        color_mode=color_mode,
    )

    return (train_ds, val_ds)

def init_model(img_width, img_height, color_mode, num_classes):
    model = keras.Sequential([
        keras.Input(shape=(img_height, img_width, 3 if color_mode == 'rgb' else 1)),
        keras.layers.Rescaling(1./255),
        keras.layers.Conv2D(32, 3, activation='relu'),
        keras.layers.MaxPooling2D(),
        keras.layers.Conv2D(32, 3, activation='relu'),
        keras.layers.MaxPooling2D(),
        keras.layers.Conv2D(32, 3, activation='relu'),
        keras.layers.MaxPooling2D(),
        keras.layers.Flatten(),
        keras.layers.Dense(256, activation='relu'),
        keras.layers.Dense(128, activation='relu'),
        keras.layers.Dense(64, activation='relu'),
        keras.layers.Dense(32, activation='relu'),
        keras.layers.Dense(16, activation='relu'),
        keras.layers.Dense(num_classes),
    ])

    model.compile(
      optimizer='adam',
      loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True),
      metrics=['accuracy'],
    )

    return model

def train_model(model, train_ds, val_ds, epochs):
    model.fit(
        train_ds,
        validation_data=val_ds,
        epochs=epochs,
    )

def main():
    parser = argparse.ArgumentParser(
        prog='Train Model',
        description='Train a model on the dataset.',
    )
    parser.add_argument(
        '-d',
        '--dataset',
        help='Path to the dataset. Otherwise, use the DATASET_PATH environment variable.',
        default=os.getenv('DATASET_PATH'),
    )
    parser.add_argument(
        '-m',
        '--model',
        help='Path to save the model. Otherwise, use the MODEL_PATH environment variable.',
        default=os.getenv('MODEL_PATH'),
    )
    parser.add_argument(
        '-e',
        '--epochs',
        help='Amount of epochs.',
        default=10,
    )

    args = parser.parse_args()

    batch_size = int(os.getenv('BATCH_SIZE'))
    img_width = int(os.getenv('IMAGE_WIDTH'))
    img_height = int(os.getenv('IMAGE_HEIGHT'))
    color_mode = os.getenv('COLOR_MODE')

    train_ds, val_ds = get_dataset(
        args.dataset,
        img_width,
        img_height,
        batch_size,
        color_mode,
    )

    class_names = train_ds.class_names
    num_classes = len(class_names)

    model = init_model(img_width, img_height, color_mode, num_classes)
    train_model(model, train_ds, val_ds, args.epochs)
    model.save(args.model)

if __name__ == '__main__':
    main()
