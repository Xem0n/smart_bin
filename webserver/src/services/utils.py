import io
import os
import numpy as np
from PIL import Image

def transform_image(id, bytes):
    size = (os.getenv('IMG_WIDTH'), os.getenv('IMG_HEIGHT'))

    image = Image.open(io.BytesIO(bytes))
    image = image.resize(size, resample=Image.Resampling.LANCZOS)
    image = image.convert(get_color_mode())
    image.save(f'tmp/{id}_transformed.jpg')

    return np.expand_dims(np.array(image), 0)

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
