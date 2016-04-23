from models import *
from config import app
from flask import session,redirect,request
from urllib.parse import unquote
import views.tool as tool
import json

def log(sss):
    app.logger.debug(sss)

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