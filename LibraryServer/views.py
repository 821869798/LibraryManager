from models import db
from config import app

@app.route("/login",methods=["POST","GET"])
def login():
    return "hello"