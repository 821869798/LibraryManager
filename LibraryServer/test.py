from manage import db
from models import *
import json
from urllib.parse import unquote

def strtoint(s, default):
    try:
        s = int(s)
    except ValueError and TypeError:
        s = default
    return s

# root = RootAdminer.query.filter_by(username="root")
# root = root.filter_by(password="e10adc3949ba59abbe56e057f20f883e").first()


# adminer = Adminer(username="admin1",name="张小三")
# db.session.add(adminer)
# db.session.commit()
# dic = {"alldata":Adminer.getall()}
# print(json.dumps(dic))

# adminer = Adminer.query.filter_by(username="admin1")
# adminer = adminer.filter_by(name="张小三").all()
# adminer = Adminer()
# print(type(adminer) is Adminer)
# print(adminer)
# print(BookType.query.offset(1).limit(6).count())

print(Book.query.get(1).booktype.name)