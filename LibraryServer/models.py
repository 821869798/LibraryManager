from config import db

class RootAdminer(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(20),unique=True,nullable=False)
    password = db.Column(db.String(20),nullable=False)


class Adminer(db.Model):
    id = db.Column(db.Integer,primary_key=True)
    username = db.Column(db.String(20),unique=True,nullable=False)
    password = db.Column(db.String(20),nullable=False)
    bookmanage = db.Column(db.Integer,nullable=False,default=0)
    readermanage = db.Column(db.Integer,nullable=False,default=0)
    name = db.Column(db.String(64))
