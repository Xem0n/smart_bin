from PIL import Image

def resize(image_path, size):
    print('resize')
    with Image.open(image_path) as img:
        img = img.resize(size, resample=Image.Resampling.LANCZOS)
        img.save(image_path)

def unify_bin_data(bins, categories, garbages):
    return [
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
                        for garbage in garbages if garbage.category_id == category.id
                    ],
                }
                for category in categories if category.bin_id == bin.id
            ],
        }
        for bin in bins
    ]
