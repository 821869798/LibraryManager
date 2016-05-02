from models import *
from config import app
from flask import session,redirect,request
from urllib.parse import unquote
import views.tool as tool
import json
import datetime
import decimal

def log(sss):
    app.logger.debug(sss)

#添加借书
@app.route("/borrow/add",methods=["POST"])
def borrow_add():
    jsonData = request.form.get("data")
    if jsonData:
        jsonDict = json.loads(unquote(jsonData))
        username = jsonDict.get("username")
        password = jsonDict.get("password")
        booklist = jsonDict.get("booklist")
        if username and password and  booklist:
            reader = Reader.query.filter_by(barcode=username,password=password).first()
            if reader:
                repDict = {"barcode":reader.barcode,"name":reader.name,"rtype":reader.rtype,"avaliable":reader.avaliable}
                if reader.avaliable is 0:
                    return json.dumps(repDict)
                else:
                    replist = []
                    borrowlist = reader.borrows.all()
                    borrowCount = reader.borrows.count()
                    for i,value in enumerate(borrowlist):
                        borrowlist[i] = value.book
                    for barcode in booklist:
                        if borrowCount > Reader.maxBorrow:
                            break
                        book = Book.query.filter_by(barcode=barcode).first()
                        if book and book not in borrowlist and book.allcount-book.borrows.count() > 0:
                            borrow = Borrow()
                            borrow.reader,borrow.book = reader,book
                            borrow.bdate = datetime.date.today()
                            borrow.rdate = borrow.bdate+datetime.timedelta(book.booktype.borrownum)
                            if reader.rtype is 1:
                                borrow.renew = 2
                            else:
                                borrow.renew = 1
                            replist.append([book.barcode,book.name,book.author,book.allcount-book.borrows.count()+1,\
                                str(borrow.bdate),str(borrow.rdate),borrow.renew])
                            db.session.add(borrow)
                            borrowCount += 1
                    db.session.commit()
                    if replist:
                        repDict["borrowlist"] = replist
                    return json.dumps(repDict)

    return "false"

#还书
@app.route("/borrow/return",methods=["POST"])
def borrow_return():
    jsonData = request.form.get("data")
    if jsonData:
        jsonDict = json.loads(unquote(jsonData))
        username = jsonDict.get("username")
        password = jsonDict.get("password")
        booklist = jsonDict.get("booklist")
        if username and password and  booklist:
            reader = Reader.query.filter_by(barcode=username,password=password).first()
            if reader:
                replist = []
                repDict = {"barcode":reader.barcode,"name":reader.name,"rtype":reader.rtype}
                for barcode in booklist:
                    book = Book.query.filter_by(barcode=barcode).first()
                    if book:
                        borrow = book.borrows.filter_by(reader=reader).first()
                        if borrow:
                            history = History(reader=reader,book=book)
                            # History.reader_ = reader.id
                            # History.book = book
                            history.bdate = borrow.bdate
                            history.rdate = datetime.date.today()
                            days = (history.rdate-borrow.rdate).days
                            if days<0:
                                days = 0
                            history.overdue = days
                            db.session.delete(borrow)
                            db.session.add(history)
                            if days is 0:
                                days = "无"
                            else:
                                reader.arrears += decimal.Decimal("%f"%(days*0.1))
                                days = ""+str(days)+"天"
                            replist.append([book.barcode,book.name,book.author,str(history.bdate),str(history.rdate),days])
                db.session.add(reader)
                db.session.commit()
                if replist:
                    repDict["returnlist"] = replist
                return json.dumps(repDict)                         
    return "false"

#获取借阅列表
@app.route("/borrow/getborrow",methods=["GET"])
def borrow_getborrow():
    logintype = tool.strtoint(session.get("logintype"),-1)
    username = session.get("username")
    if logintype is 0 and username:
        reader = Reader.query.filter_by(barcode=username).first()
        if reader:
            borrowlist = reader.borrows.all()
            for i,value in enumerate(borrowlist):
                borrowlist[i] = value.toArrayData()
            return json.dumps(borrowlist)
    return "false"

#获取历史记录
@app.route("/borrow/history",methods=["GET"])
def borrow_history():
    logintype = tool.strtoint(session.get("logintype"),-1)
    username = session.get("username")
    page = tool.strtoint(request.args.get("page"),0)
    if logintype is 0 and username:
        reader = Reader.query.filter_by(barcode=username).first()
        if reader:
            historylist = reader.historys.offset(History.pageCount*page).limit(History.pageCount).all()
            for i,value in enumerate(historylist):
                historylist[i] = value.toArrayData()
            return json.dumps(historylist)
    return "false"

@app.route("/borrow/renew",methods=["POST"])
def borrow_renew():
    logintype = tool.strtoint(session.get("logintype"),-1)
    username = session.get("username")
    borrowlist = request.form.get("data")
    if logintype is 0 and username and borrowlist:
        borrowlist = unquote(borrowlist)
        borrowlist = json.loads(borrowlist)
        reader = Reader.query.filter_by(barcode=username).first()
        if reader:
            count = 0
            for barcode in borrowlist:
                book = Book.query.filter_by(barcode=barcode).first()
                borrow = reader.borrows.filter_by(book=book).first()
                if borrow and borrow.renew > 0 :
                    borrow.renew -= 1
                    borrow.rdate = borrow.rdate+datetime.timedelta(30)
                    count += 1
                    db.session.add(borrow)
            db.session.commit()
            return ("%d" % count)
    return "false"