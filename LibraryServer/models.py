from config import db
from datetime import datetime, time, date

defaultPwd = 'e10adc3949ba59abbe56e057f20f883e'

# 超级管理


class RootAdminer(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(20), unique=True, nullable=False)
    password = db.Column(db.String(32), nullable=False)

# 普通管理


class Adminer(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(20), unique=True, nullable=False)
    password = db.Column(db.String(32), nullable=False, default=defaultPwd)
    bookmanage = db.Column(db.Integer, nullable=False, default=0)
    readermanage = db.Column(db.Integer, nullable=False, default=0)
    name = db.Column(db.String(64))

    def toArrayData(self):
        arr = [self.username, self.name]
        arr.append(self.bookmanage and "允许" or "不允许")
        arr.append(self.readermanage and "允许" or "不允许")
        return arr

    @staticmethod
    def getall(datalist):
        for i in range(len(datalist)):
            datalist[i] = datalist[i].toArrayData()
        return datalist

# 图书类型


class BookType(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(64), nullable=False,unique=True)
    borrownum = db.Column(db.Integer, nullable=False, default=30)
    books = db.relationship('Book', backref='booktype', lazy='dynamic')

    def toArrayData(self):
        arr = [self.id, self.name,self.borrownum]
        return arr;

    @staticmethod
    def getall():
        datalist = BookType.query.all()
        for i in range(len(datalist)):
            datalist[i] = datalist[i].toArrayData()
        return datalist

# 图书


class Book(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    barcode = db.Column(db.String(20), nullable=False, unique=True)
    name = db.Column(db.String(64), nullable=False)
    booktype_id = db.Column(db.Integer, db.ForeignKey("book_type.id"))
    author = db.Column(db.String(64))
    press = db.Column(db.String(64))
    date = db.Column(db.Date)
    price = db.Column(db.Numeric(10, 2), nullable=False, default=0)
    page = db.Column(db.Integer, nullable=False, default=0)
    bookshelf = db.Column(db.Integer)
    allcount = db.Column(db.Integer, name=False)
    borrows = db.relationship("Borrow", backref="book", lazy="dynamic")
    historys = db.relationship("History", backref="book", lazy="dynamic")
    # losebooks = db.relationship("LoseBook", backref="book", lazy="dynamic")

    pageCount = 10

    def toArrayData(self):
        arr = [self.barcode, self.name,self.author,self.booktype.name,self.allcount,self.allcount-self.borrows.count()]
        return arr

    def toCompleteData(self):
        arr = [self.barcode,self.name,self.booktype.id,self.author,self.press,str(self.date), \
        str(self.price),self.page,self.bookshelf,self.allcount,self.allcount-self.borrows.count()]
        return arr

    @staticmethod
    def getsome(datalist):
        for i in range(len(datalist)):
            datalist[i] = datalist[i].toArrayData()
        return datalist


# 有效证件


class License(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(64), nullable=False,unique=True)
    readers = db.relationship("Reader", backref="license", lazy="dynamic")

    def toArrayData(self):
        arr = [self.id,self.name]
        return arr

    @staticmethod
    def getall():
        datalist = License.query.all()
        for i in range(len(datalist)):
            datalist[i] = datalist[i].toArrayData()
        return datalist

# 读者信息


class Reader(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    barcode = db.Column(db.String(20), nullable=False, unique=True)
    name = db.Column(db.String(64), nullable=False)
    sex = db.Column(db.String(2), nullable=False, default="男")
    rtype = db.Column(db.Integer, nullable=False, default=0)
    date = db.Column(db.Date)
    license_id = db.Column(db.Integer, db.ForeignKey("license.id"))
    licensenum = db.Column(db.String(64))
    phone = db.Column(db.String(64))
    email = db.Column(db.String(64))
    arrears = db.Column(db.Numeric(10, 2), nullable=False, default=0)
    note = db.Column(db.String(128))
    password = db.Column(db.String(32), nullable=False, default=defaultPwd)
    avaliable = db.Column(db.Integer,nullable=False,default=1)
    borrows = db.relationship("Borrow", backref="reader", lazy="dynamic")
    historys = db.relationship("History", backref="reader", lazy="dynamic")
    # losebooks = db.relationship("LoseBook", backref="reader", lazy="dynamic")
    pageCount = 10

    def toArrayData(self):
        arr = [self.barcode,self.name,self.sex,self.rtype,str(self.date),\
        self.license.name,self.licensenum,self.phone,self.email,self.avaliable,self.note,float(self.arrears)]
        return arr

    @staticmethod
    def getsome(datalist):
        for i in range(len(datalist)):
            datalist[i] = datalist[i].toArrayData()
        return datalist


# 借阅信息


class Borrow(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    book_id = db.Column(db.Integer, db.ForeignKey("book.id"))
    reader_id = db.Column(db.Integer, db.ForeignKey("reader.id"))
    bdate = db.Column(db.Date, nullable=False, default=date.today())
    rdate = db.Column(db.Date)
    renew = db.Column(db.Integer, nullable=False, default=1)

# 借阅历史


class History(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    book_id = db.Column(db.Integer, db.ForeignKey("book.id"))
    reader_id = db.Column(db.Integer, db.ForeignKey("reader.id"))
    bdate = db.Column(db.Date, nullable=False)
    rdate = db.Column(db.Date, nullable=False, default=date.today())
    overdue = db.Column(db.Integer, nullable=False, default=0)
    pageCount = 10
# 遗失图书


# class LoseBook(db.Model):
#     id = db.Column(db.Integer, primary_key=True)
#     book_id = db.Column(db.Integer, db.ForeignKey("book.id"))
#     reader_id = db.Column(db.Integer, db.ForeignKey("reader.id"))
#     bdate = db.Column(db.Date, nullable=False)
#     ldate = db.Column(db.Date, nullable=False, default=date.today())
