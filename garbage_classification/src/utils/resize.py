from PIL import Image
from os import path
from dotenv import load_dotenv
import os
import argparse

load_dotenv()

DEST_IMG_WIDTH = int(os.getenv('IMAGE_WIDTH'))
DEST_IMG_HEIGHT = int(os.getenv('IMAGE_HEIGHT'))

def resize_directory(source_directory, target_directory, mode, grayscale):
    files = [f for f in os.listdir(source_directory) if path.isfile(path.join(source_directory, f))]

    for file in files:
        with Image.open(path.join(source_directory, file)) as img:
            img = resize(img, mode)

            if grayscale:
                img = img.convert('L')

            img.save(path.join(target_directory, file))

def resize(img, mode):
    if mode == 'fill':
        return resize_and_fill(img)
    elif mode == 'scale':
        return resize_and_scale(img)

def resize_and_fill(img, fill_color=(255, 255, 255)):
    width, height = get_scaled_size(img)

    img = img.resize(
        (width, height),
        resample=Image.Resampling.LANCZOS,
    )

    resized_image = Image.new('RGB', (DEST_IMG_WIDTH, DEST_IMG_HEIGHT), fill_color)
    resized_image.paste(
        img,
        (
            (DEST_IMG_WIDTH - img.width) // 2,
            (DEST_IMG_HEIGHT - img.height) // 2
        )
    )

    return resized_image

def get_scaled_size(img):
    width, height = img.width, img.height
    aspect_ratio = height / width

    # im sure theres better way xd
    if aspect_ratio > 1:
        width = DEST_IMG_HEIGHT / aspect_ratio
        height = DEST_IMG_HEIGHT
    elif aspect_ratio < 1:
        width = DEST_IMG_WIDTH
        height = DEST_IMG_WIDTH * aspect_ratio
    else:
        width = min(DEST_IMG_WIDTH, DEST_IMG_HEIGHT)
        height = width

    return (int(width), int(height))

def resize_and_scale(img):
    img = img.resize(
        (DEST_IMG_WIDTH, DEST_IMG_HEIGHT),
        resample=Image.Resampling.LANCZOS,
    )
    img = remove_alpha(img)

    return img

def remove_alpha(img):
    return img.convert('RGB')

def main():
    parser = argparse.ArgumentParser(
        prog='Resize Images',
        description='Resize every image in the directory.',
    )
    parser.add_argument(
        '-s',
        '--source',
        help='Directory of images to resize.',
        required=True,
    )
    parser.add_argument(
        '-t',
        '--target',
        help='Where to save the resized images.',
        required=True,
    )
    parser.add_argument(
        '-m',
        '--mode',
        help='Resize technique.',
        default='fill',
        choices=['fill', 'scale'],
    )
    parser.add_argument(
        '-g',
        '--grayscale',
        help='Whether should be grayscale.',
        default=False,
        action='store_true',
    )

    args = parser.parse_args()

    resize_directory(args.source, args.target, args.mode, args.grayscale)

if __name__ == '__main__':
    main()
