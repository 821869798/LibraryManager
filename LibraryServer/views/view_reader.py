from models import *
from config import app
from flask import session,redirect,request
from urllib.parse import unquote
import views.tool as tool
import json
import decimal

def log(sss):
    app.logger.debug(sss)

@app.route("/license/getall",methods=["GET","POST"])
def license_getall():
    return json.dumps(License.getall())

#读者查询 0->名字,1->读者条形码
@app.route("/reader/query",methods=["GET"])
def reader_query():
    if tool.readermanageValid(session):
        page = tool.strtoint(request.args.get("page"),0)
        query_type = tool.strtoint(request.args.get("type"),0)
        query_str = request.args.get("query")
        readerlist = Reader.query
        if query_str:
            query_str = unquote(query_str)
            if query_type is 0:
                readerlist = readerlist.filter(Reader.name.ilike("%"+query_str+"%"))
            elif query_type is 1:
                readerlist = readerlist.filter(Reader.barcode==query_str)
        readerlist = readerlist.offset(Reader.pageCount*page).limit(Reader.pageCount).all()
        return json.dumps(Reader.getsome(readerlist))
    return "false"

#管理员获取读者信息
@app.route("/reader/getone",methods=["GET"])
def reader_getone():
    if tool.readermanageValid(session):
        barcode = request.args.get("barcode")
        if barcode:
            reader = Reader.query.filter_by(barcode=barcode).first()
            if reader:
                return json.dumps(reader.toArrayData()) 
    return "false"

#更改读者信息
@app.route("/reader/change",methods=["POST"])
def reader_change():
    if tool.readermanageValid(session):
        data = request.form.get("data")
        if data:
            arr = json.loads(unquote(data))
            barcode = arr[10]
            reader = Reader.query.filter_by(barcode=barcode).first()
            if reader and (arr[0]==arr[10] or Reader.query.filter_by(barcode=arr[0]).count() is 0):
                reader.barcode = arr[0]
                reader.name = arr[1]
                reader.sex = arr[2]
                reader.rtype = arr[3]
                reader.date = datetime.strptime(arr[4],'%Y-%m-%d')
                reader.license_id = arr[5]
                reader.licensenum = arr[6]
                reader.phone = arr[7]
                reader.email = arr[8]
                reader.note = arr[9]
                db.session.add(reader)
                db.session.commit()
                return "true"
    return "false";

#增加读者
@app.route("/reader/new",methods=["POST"])
def reader_new():
    if tool.readermanageValid(session):
        data = request.form.get("data")
        if data:
            arr = json.loads(unquote(data))
            barcode = arr[0]
            if Reader.query.filter_by(barcode=barcode).count() is 0:
                reader = Reader()
                reader.barcode = barcode
                reader.name = arr[1]
                reader.sex = arr[2]
                reader.rtype = arr[3]
                reader.date = datetime.strptime(arr[4],'%Y-%m-%d')
                reader.license_id = arr[5]
                reader.licensenum = arr[6]
                reader.phone = arr[7]
                reader.email = arr[8]
                reader.note = arr[9]
                db.session.add(reader)
                db.session.commit()
                return "true"
    return "false"

#删除读者
@app.route("/reader/delete",methods=["GET"])
def reader_delete():
    if tool.readermanageValid(session):
        barcode = request.args.get("barcode")
        if barcode:
            reader = Reader.query.filter_by(barcode=barcode).first()
            if reader:
                borrows = reader.borrows.all()
                for borrow in borrows:
                    db.session.delete(borrow)
                historys = reader.historys.all()
                for history in historys:
                    db.session.delete(history)
                db.session.delete(reader)
                db.session.commit()
                return "true"
    return "false"

#更改其他信息
@app.route("/reader/change/other",methods=["POST"])
def reader_change_other():
    if tool.readermanageValid(session):
        barcode = request.form.get("barcode")
        data = request.form.get("data")
        if barcode and data:
            reader = Reader.query.filter_by(barcode=barcode).first()
            dataDict = json.loads(data)
            if reader:
                if "avaliable" in dataDict:
                    reader.avaliable = dataDict["avaliable"]
                if "arrears" in dataDict:
                    reader.arrears = dataDict["arrears"]
                db.session.add(reader)
                db.session.commit()
                return "true"
    return "false"

#读者自己获取信息
@app.route("/reader/getself",methods=["GET"])
def reader_getself():
    logintype = tool.strtoint(session.get("logintype"),-1)
    username = session.get("username")
    if logintype is 0 and username:
        reader = Reader.query.filter_by(barcode=username).first()
        if reader:
            return json.dumps(reader.toArrayData())
    return "false"

@app.route("/reader/changeself",methods=["POST"])
def reader_changeself():
    logintype = tool.strtoint(session.get("logintype"),-1)
    username = session.get("username")
    if logintype is 0 and username:
        reader = Reader.query.filter_by(barcode=username).first()
        if reader:
            email = request.form.get("email")
            phone = request.form.get("phone")
            if email is not None and phone is not None:
                reader.email = email
                reader.phone = phone
                db.session.add(reader)
                db.session.commit()
                return "true"
    return "false"

# @app.route("/test1",methods=["GET"])
# def test1():
#     arg1 = request.args.get("a")
#     arg2 = request.args.get("b")
    
#     if arg1 is not None and arg2 is not None:
#         log("arg1:"+arg1)
#         log("arg2:"+arg2)
        
#     return "false"