from models import *
from config import app
from flask import session
from flask import request
from flask import make_response

@app.route("/login",methods=["POST","GET"])
def login():
    app.logger.debug(request.form)
    username = request.form.get("username")
    password = request.form.get("password")
    logintype = request.form.get("logintype")
    if username and password and logintype:
        logintype = int(logintype)
        if logintype is 2:
            root = RootAdminer.query.filter_by(username=username,password=username).first()
            if root:
                session["username"] = username
                session["logintype"] = 2
                return "true"
        elif logintype is 1:
            pass
        else:
            pass
    return "false"

@app.route("/test")
def test():
    return "学霸"
