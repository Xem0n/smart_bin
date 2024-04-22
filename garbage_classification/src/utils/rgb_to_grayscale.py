from PIL import Image
from os import path
import os
import argparse

def convert_directory_to_grayscale(source_directory, target_directory):
    files = [f for f in os.listdir(source_directory) if path.isfile(path.join(source_directory, f))]

    for file in files:
        with Image.open(path.join(source_directory, file)) as img:
            img = rgb_to_grayscale(img)
            img.save(path.join(target_directory, file))

def rgb_to_grayscale(img):
    return img.convert('L')

def main():
    parser = argparse.ArgumentParser(
        prog='RGB to Grayscale',
        description='Convert directory of RGB images to Grayscale.',
    )
    parser.add_argument(
        '-s',
        '--source',
        help='Directory of RGB images to convert to Grayscale.',
        required=True,
    )
    parser.add_argument(
        '-t',
        '--target',
        help='Where to save the converted images.',
        required=True,
    )

    args = parser.parse_args()

    convert_directory_to_grayscale(args.source, args.target)

if __name__ == '__main__':
    main()
