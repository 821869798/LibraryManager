from flask.ext.sqlalchemy import SQLAlchemy
from flask import Flask
import os
basedir = os.path.abspath(os.path.dirname(__file__))
app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] =\
'sqlite:///' + os.path.join(basedir, 'librarydb.sqlite')
app.config['SQLALCHEMY_COMMIT_ON_TEARDOWN'] = True
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = True
app.secret_key = '\x91\xeb/\xd0+\xb4\xcc\\\x01\xff\x19\rR=\x0f6\x08\x8eJy\x90\x80\xb9?'
db = SQLAlchemy(app)