import uuid
import time
from db.models import Bin, Category, Garbage
from db.database import db_session

def init_bin(mac_address):
    bin = Bin(id=mac_address, name='Śmietnik', color='#e5b514')
    bin.categories.append(Category(id=str(uuid.uuid4()), type_id=1, name='Metal', color='#F94144'))
    bin.categories.append(Category(id=str(uuid.uuid4()), type_id=2, name='Papier', color='#5AA9E7'))
    bin.categories.append(Category(id=str(uuid.uuid4()), type_id=3, name='Plastik', color='#E5B514'))

    db_session.add(bin)
    db_session.commit()

def add_garbage(mac_address, garbage_type):
    if mac_address == '' or mac_address is None:
        return

    bin = db_session.query(Bin).filter(Bin.id == mac_address).first()

    if bin is None:
        init_bin(mac_address)

    category = db_session.query(Category).filter(Category.bin_id == mac_address, Category.type_id == int(garbage_type) + 1).first()
    garbage = Garbage(id=str(uuid.uuid4()), category_id=category.id, created_at=int(time.time()))

    db_session.add(garbage)
    db_session.commit()

def get_bins_data():
    bins = db_session.query(Bin).all()
    categories = db_session.query(Category).all()
    garbages = db_session.query(Garbage).all()

    return bins, categories, garbages

def update_bin(mac_address, name, color):
    db_session.query(Bin).filter(Bin.id == mac_address).update({Bin.name: name, Bin.color: color})
    db_session.commit()
