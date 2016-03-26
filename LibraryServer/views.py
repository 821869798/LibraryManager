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
        return "正确"

    return "错误"