import io
import os
import numpy as np
from PIL import Image
from keras.preprocessing import image

size = (int(os.getenv('IMG_WIDTH')), int(os.getenv('IMG_HEIGHT')))

def transform_image(id, bytes):
    img = Image.open(io.BytesIO(bytes))
    img = img.resize(size, resample=Image.Resampling.LANCZOS)
    img = img.convert(get_color_mode())
    img.save(f'tmp/{id}_transformed.jpg')

    return np.expand_dims(image.img_to_array(img, dtype=np.uint8), 0)

def get_color_mode():
    return 'RGB' if os.getenv('COLOR_MODE') == 'rgb' else 'L'

def get_bin_data_dto(bins):
    return {
        'bins': [
            {
                'id': bin.id,
                'name': bin.name,
                'color': bin.color,
                'categories': [
                    {
                        'id': category.id,
                        'name': category.name,
                        'color': category.color,
                        'garbages': [
                            {
                                'id': garbage.id,
                                'created_at': garbage.created_at,
                            }
                            for garbage in category.garbages
                        ],
                    }
                    for category in bin.categories
                ],
            }
            for bin in bins
        ]
    }
