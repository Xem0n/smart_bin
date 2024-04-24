from PIL import Image
from os import path
from dotenv import load_dotenv
import os
import argparse

load_dotenv()

DEST_IMG_WIDTH = int(os.getenv('IMAGE_WIDTH'))
DEST_IMG_HEIGHT = int(os.getenv('IMAGE_HEIGHT'))

def resize_directory(source_directory, target_directory):
    files = [f for f in os.listdir(source_directory) if path.isfile(path.join(source_directory, f))]

    for file in files:
        with Image.open(path.join(source_directory, file)) as img:
            img = resize(img)
            img.save(path.join(target_directory, file))

def resize(img):
    if args.mode == 'fill':
        return resize_and_fill(img)
    elif args.mode == 'scale':
        return resize_and_scale(img)

def resize_and_fill(img, fill_color=(255, 255, 255)):
    if img.width > DEST_IMG_WIDTH or img.height > DEST_IMG_HEIGHT:
        min_dimension = min(DEST_IMG_WIDTH, DEST_IMG_HEIGHT)
        img.thumbnail((min_dimension, min_dimension), Image.LANCZOS)

    resized_image = Image.new('RGB', (DEST_IMG_WIDTH, DEST_IMG_HEIGHT), fill_color)
    resized_image.paste(
        img,
        (
            (DEST_IMG_WIDTH - img.width) // 2,
            (DEST_IMG_HEIGHT - img.height) // 2
        )
    )

    return resized_image

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

    args = parser.parse_args()

    resize_directory(args.source, args.target)

if __name__ == '__main__':
    main()
