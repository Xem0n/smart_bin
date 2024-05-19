from typing import List
from sqlalchemy import String, ForeignKey
from sqlalchemy.orm import Mapped, mapped_column, relationship
from db.database import Base

class Bin(Base):
    __tablename__ = 'bin'

    id: Mapped[str] = mapped_column(String(12), primary_key=True)
    name: Mapped[str] = mapped_column(String(64))
    color: Mapped[str] = mapped_column(String(9))

    categories: Mapped[List['Category']] = relationship(back_populates='bin', cascade='all, delete-orphan')

class Category(Base):
    __tablename__ = 'category'

    id: Mapped[str] = mapped_column(String(36), primary_key=True)
    name: Mapped[str] = mapped_column(String(64))
    color: Mapped[str] = mapped_column(String(9))
    bin_id: Mapped[str] = mapped_column(String(12), ForeignKey('bin.id'))

    bin: Mapped['Bin'] = relationship(back_populates='categories')
    garbages: Mapped[List['Garbage']] = relationship(back_populates='category', cascade='all, delete-orphan')

class Garbage(Base):
    __tablename__ = 'garbage'

    id: Mapped[str] = mapped_column(String(36), primary_key=True)
    created_at: Mapped[int] = mapped_column(String(36))
    category_id: Mapped[str] = mapped_column(String(36), ForeignKey('category.id'))

    category: Mapped['Category'] = relationship(back_populates='garbages')
