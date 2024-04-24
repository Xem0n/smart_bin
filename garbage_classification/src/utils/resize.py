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
            img = remove_alpha(img)
            img.save(path.join(target_directory, file))

def resize(img):
    return img.resize(
        (DEST_IMG_WIDTH, DEST_IMG_HEIGHT),
        resample=Image.Resampling.LANCZOS,
    )

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

    args = parser.parse_args()

    resize_directory(args.source, args.target)

if __name__ == '__main__':
    main()
