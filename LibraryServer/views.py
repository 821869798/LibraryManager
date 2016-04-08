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


@app.route("/adminer/getall", methods=["GET","POST"])
def adminer_getall():
    logintype = session.get("logintype")
    if logintype:
        logintype = tool.strtoint(logintype, -1)
        if logintype is 2:
            dic = {"alldata": Adminer.getall(Adminer.query.all())}
            return json.dumps(dic)
    return "false"


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

@app.route("/booktype/getall",methods=["GET","POST"])
def booktype_getall():
    dic = {"alldata":BookType.getall()}
    return json.dumps(dic)

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

@app.route("/book/getsome",methods=["GET"])
def book_getsome():
    page = tool.strtoint(request.args.get("page"),0)
    return json.dumps(Book.getsome(page))

@app.route("/test")
def test():
    abc = request.args.get("test")
    abc = json.loads(abc)
    app.logger.debug(abc)
    return "abc"
