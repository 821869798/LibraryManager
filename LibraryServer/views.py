from models import db
from config import app
from flask import session
from flask import request
from flask import make_response

@app.route("/login",methods=["POST","GET"])
def login():
    return "hello"