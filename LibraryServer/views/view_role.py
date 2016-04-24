from models import *
from config import app
from flask import session,redirect,request
from urllib.parse import unquote
import views.tool as tool
import json

def log(sss):
    app.logger.debug(sss)


#登陆
@app.route("/role/login", methods=["POST"])
def login():
    username = request.form.get("username")
    password = request.form.get("password")
    logintype = request.form.get("logintype")
    if username and password and logintype:
        username = unquote(username)
        logintype = tool.strtoint(logintype, -1)
        app.logger.debug(username + " " + password+" "+str(logintype))
        if logintype is 2:
            root = RootAdminer.query.filter_by(
                username=username, password=password).first()
            if root:
                session["username"] = username
                session["logintype"] = 2
                dic = {"logintype":2}
                return json.dumps(dic)
        elif logintype is 1:
            adminer = Adminer.query.filter_by(username=username,password=password).first()
            if adminer:
                session["username"] = username
                session["logintype"] = 1
                dic = {"logintype":1,"book":adminer.bookmanage,"reader":adminer.readermanage}
                return json.dumps(dic)
        elif logintype is 0:
            reader = Reader.query.filter_by(barcode=username,password=password).first()
            if reader:
                session["username"] = username
                session["logintype"] = 0
                dic = {"logintype":0 ,"borrow":reader.avaliable}
                return json.dumps(dic)
    return "false"


@app.route("/role/changepwd",methods=["POST"])
def role_changepwd():
    logintype = session.get("logintype")
    username = session.get("username")

    if logintype and username:
        old_pwd = request.form.get("old")
        new_pwd = request.form.get("new")
        logintype = tool.strtoint(logintype,-1)
        if logintype is 2:
            role = RootAdminer.query.filter_by(username=username).first()
        elif logintype is 1:
            role = Adminer.query.filter_by(username=username).first()
        elif logintype is 0:
            role = Reader.query.filter_by(barcode=username).first()
        if role and role.password==old_pwd:
            role.password = new_pwd
            db.session.add(role)
            db.session.commit()
            return "true"
    return "false"