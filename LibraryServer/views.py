from models import *
from config import app
from flask import session
from flask import request
from flask import make_response
from urllib.parse import unquote
import json
import tool

def log(sss):
    app.logger.debug(sss)

#登陆
@app.route("/login", methods=["POST"])
def login():
    username = request.form.get("username")
    password = request.form.get("password")
    logintype = request.form.get("logintype")
    if username and password and logintype:
        username = unquote(username)
        app.logger.debug(username + " " + password)
        logintype = tool.strtoint(logintype, -1)
        if logintype is 2:
            root = RootAdminer.query.filter_by(
                username=username, password=password).first()
            if root:
                session["username"] = username
                session["logintype"] = 2
                return "true"
        elif logintype is 1:
            pass
        elif logintype is 0:
            pass
    return "false"

#获取所有管理员
@app.route("/adminer/getall", methods=["GET","POST"])
def adminer_getall():
    logintype = session.get("logintype")
    if logintype:
        logintype = tool.strtoint(logintype, -1)
        if logintype is 2:
            dic = {"alldata": Adminer.getall(Adminer.query.all())}
            return json.dumps(dic)
    return "false"

#添加管理员
@app.route("/adminer/new", methods=["POST"])
def adminer_new():
    logintype = session.get("logintype")
    if logintype:
        logintype = tool.strtoint(logintype, -1)
        if logintype is 2:
            username = request.form.get("username")
            name = request.form.get("name")
            password = request.form.get("password")
            bookmanage = request.form.get("bookmanage")
            readermanage = request.form.get("readermanage")
            if username and name and bookmanage and readermanage:
                username = unquote(username)
                name = unquote(name)
                if not Adminer.query.filter_by(username=username).first():
                    bookmanage = tool.strtoint(bookmanage, 0)
                    readermanage = tool.strtoint(readermanage, 0)
                    adminer = Adminer(
                        username=username, name=name, bookmanage=bookmanage, readermanage=readermanage)
                    if password:
                        adminer.password = password
                    db.session.add(adminer)
                    db.session.commit()
                    return "true"
    return "false"

#更改管理员权限
@app.route("/adminer/set_manage", methods=["POST"])
def adminer_set_bookmanage():

    logintype = session.get("logintype")
    if logintype:
        logintype = tool.strtoint(logintype, -1)
        if logintype is 2:
            username = request.form.get("username")
            value = request.form.get("value")
            mtype = request.form.get("type")
            if username and value:
                username = unquote(username)
                value = tool.strtoint(value, 0)
                mtype = tool.strtoint(mtype, 1)
                adminer = Adminer.query.filter_by(username=username).first()
                if adminer:
                    if mtype is 1:
                        adminer.bookmanage = value
                    else:
                        adminer.readermanage = value
                    db.session.add(adminer)
                    db.session.commit()
                    return "true"
    return "false"

#查询管理员
@app.route("/adminer/query", methods=["POST"])
def adminer_query():

    logintype = session.get("logintype")
    if logintype:
        logintype = tool.strtoint(logintype, -1)
        if logintype is 2:
            username = request.form.get("username")
            name = request.form.get("name")
            root = Adminer.query;
            if username:
                username = unquote(username)
                root = root.filter(Adminer.username==username)
            if name:
                name = unquote(name)
                root = root.filter(Adminer.name.like("%"+name+"%"))
            dic = {"alldata": Adminer.getall(root.all())}
            return json.dumps(dic)
    return "false"

#删除管理员
@app.route("/adminer/delete", methods=["POST"])
def adminer_delete():
    logintype = session.get("logintype")
    if logintype:
        logintype = tool.strtoint(logintype, -1)
        if logintype is 2:
            username = request.form.get("username")
            if username:
                username = unquote(username)
                adminer = Adminer.query.filter_by(username=username).first()
                if adminer:
                    db.session.delete(adminer)
                    db.session.commit()
                    return "true"

    return "false"

#获取所有图书类型
@app.route("/booktype/getall",methods=["GET","POST"])
def booktype_getall():
    dic = {"alldata":BookType.getall()}
    return json.dumps(dic)

#添加图书类型
@app.route("/booktype/new",methods=["POST"])
def booktype_new():
    if tool.bookmanageValid(session):
        name = request.form.get("name")
        borrownum = request.form.get("borrownum")
        if name:
            name = unquote(name)
            btype = BookType(name=name)
            if borrownum:
                borrownum = tool.strtoint(borrownum,30)
                btype.borrownum = borrownum
            db.session.add(btype)
            db.session.commit()
            return "true"
    return "false"

#更改图书类型
@app.route("/booktype/change",methods=["POST"])
def booktype_change():
    if tool.bookmanageValid(session):
        bt_id = request.form.get("id")
        name = request.form.get("name")
        borrownum = tool.strtoint(request.form.get("borrownum"),0)
        if bt_id and name and borrownum>0:
            name = unquote(name)
            bt_id = tool.strtoint(bt_id,0)
            bt = BookType.query.get(bt_id)
            if bt:
                bt.name = name
                bt.borrownum = borrownum
                db.session.add(bt)
                db.session.commit()
                return "true"
    return "false"

#删除图书类型
@app.route("/booktype/delete",methods=["GET"])
def booktype_delete():
    if tool.bookmanageValid(session):
        bt_id = request.args.get("id")
        if bt_id:
            bt_id = tool.strtoint(bt_id,0)
            bt = BookType.query.get(bt_id)
            if bt and bt.books.count() is 0:
                db.session.delete(bt)
                db.session.commit()
                return "true"

    return "false"

#添加图书
@app.route("/book/new",methods=["POST"])
def book_new():
    if tool.bookmanageValid(session):
        data = request.form.get("data")
        if data:
            arr = json.loads(unquote(data))
            book = Book()
            barcode = arr[0]
            if Book.query.filter_by(barcode=barcode).count() is 0:
                book.barcode = barcode
                book.name = arr[1]
                book.booktype_id = arr[2]
                book.author = arr[3]
                book.press = arr[4]
                bdate = datetime.strptime(arr[5],'%Y-%m-%d')
                book.date = bdate
                book.price = arr[6]
                book.page = arr[7]
                book.bookshelf = arr[8]
                book.allcount = arr[9]
                db.session.add(book)
                db.session.commit()
                return "true"
    return "false"

#获取图书完整信息
@app.route("/book/getcomplete",methods=["GET"])
def book_getcomplete():
    barcode = request.args.get("barcode")
    if barcode:
        book = Book.query.filter_by(barcode=barcode).first()
        if book:
            return json.dumps(book.toCompleteData())
    return "false"

#删除图书
@app.route("/book/delete",methods=["GET"])
def book_delete():
    if tool.bookmanageValid(session):
        barcode = request.args.get("barcode")
        if barcode:
            book = Book.query.filter_by(barcode=barcode).first()
            if book and book.borrows.count() is 0:
                historys = book.historys.all()
                for data in historys:
                    db.session.delete (data)
                db.session.delete(book)
                db.session.commit()
                return "true"
    return "false"

#更改图书信息
@app.route("/book/change",methods=["POST"])
def book_change():
    if tool.bookmanageValid(session):
        data = request.form.get("data")
        if data:
            arr = json.loads(unquote(data))
            book = Book()
            barcode = arr[10]
            log(barcode)
            book = Book.query.filter_by(barcode=barcode).first()
            if book and (arr[0]==arr[10] or Book.query.filter_by(barcode=arr[0]).count() is 0):

                book.barcode = arr[0]
                book.name = arr[1]
                book.booktype_id = arr[2]
                book.author = arr[3]
                book.press = arr[4]
                bdate = datetime.strptime(arr[5],'%Y-%m-%d')
                book.date = bdate
                book.price = arr[6]
                book.page = arr[7]
                book.bookshelf = arr[8]
                if arr[9] >= book.borrows.count():
                    book.allcount = arr[9]
                    db.session.add(book)
                    db.session.commit()
                    return "true"
    return "false"

#图书查询 0->书名,1->条形码,2->作者,3->出版社,4->书架
@app.route("/book/query",methods=["GET"])
def book_query():
    page = tool.strtoint(request.args.get("page"),0)
    query_type = tool.strtoint(request.args.get("type"),0)
    btype_id = tool.strtoint(request.args.get("btype"),-1)
    query_str = request.args.get("query")
    booklist = Book.query
    if btype_id is not -1:
        booklist = booklist.filter(Book.booktype_id==btype_id)
    if query_str:
        query_str = unquote(query_str)
        if query_type is 0:
            booklist = booklist.filter(Book.name.ilike("%"+query_str+"%"))
        elif query_type is 1:
            booklist = booklist.filter(Book.barcode==query_str)
        elif query_type is 2:
            booklist = booklist.filter(Book.author.ilike("%"+query_str+"%"))
        elif query_type is 3:
            booklist = booklist.filter(Book.press.ilike("%"+query_str+"%"))
        elif query_type is 4:
            booklist = booklist.filter(Book.bookshelf==tool.strtoint(query_str,0))
    booklist = booklist.offset(Book.pageCount*page).limit(Book.pageCount).all()
    return json.dumps(Book.getsome(booklist))

@app.route("/reader/query",methods=["GET"])
def reader_query():
    if tool.readermanageValid(session):
        pass
    return "false"

@app.route("/test")
def test():
    abc = request.args.get("test")
    abc = json.loads(abc)
    app.logger.debug(abc)
    return "abc"
