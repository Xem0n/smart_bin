from PIL import Image

def resize(image_path, size):
    print('resize')
    with Image.open(image_path) as img:
        img = img.resize(size, resample=Image.Resampling.LANCZOS)
        img.save(image_path)
