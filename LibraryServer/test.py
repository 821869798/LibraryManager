from manage import db
from models import *
import json
from urllib.parse import unquote
import datetime
import decimal

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
# book = Book.query.filter_by(barcode="13").first()
# book.barcode = 14
# db.session.add(book)
# db.session.commit()
# print(book.name)
# today = datetime.date.today()
# dat = datetime.timedelta(3)
# today = today+dat
# print(today)

# root = RootAdminer(username="root1",password="123")
# db.session.add(root)
# print(root.id)
# reader = Reader.query.filter_by(barcode="1200110101").first()
# borrowlist = reader.borrows.all()
# for i,value in enumerate(borrowlist):
#     borrowlist[i] = value.book
# book = Book.query.get(1)
# print(book)
# for line in borrowlist:
#     print(line)
# if book in borrowlist:
#     print("book")
# else:
#     print("error")
# reader = Reader.query.filter_by(barcode="1200110102").first()
# book = Book.query.get(1)
# borrow = book.borrows.filter_by(reader=reader).first()
# print(borrow)
# date1 = datetime.date(2016,6,7)
# date2 = datetime.date(2016,7,10)
# print((date1-date2).days)
# barcode = "10000001"
# book = Book.query.filter_by(barcode=barcode).first()
# reader = Reader.query.filter_by(barcode="1200110101").first()
# print(reader.borrows.count())

#reader = Reader.query.filter_by(barcode="1200110101").first()
# a = 0
# reader.arrears = and
# db.session.add(reader)
# db.session.commit()
# print(reader.arrears)
print("gagag阿迪斯")