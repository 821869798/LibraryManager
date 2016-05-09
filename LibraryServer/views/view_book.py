from models import *
from config import app
from flask import session,redirect,request
from urllib.parse import unquote
import views.tool as tool
import json

def log(sss):
    app.logger.debug(sss)
    
#获取所有图书类型
@app.route("/booktype/getall",methods=["GET","POST"])
def booktype_getall():
    dic = {"alldata":BookType.getall()}
    return json.dumps(dic)

#添加图书类型
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

#更改图书类型
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

#删除图书类型
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

#添加图书
@app.route("/book/new",methods=["POST"])
def book_new():
    if tool.bookmanageValid(session):
        data = request.form.get("data")
        if data:
            arr = json.loads(unquote(data))
            barcode = arr[0]
            if Book.query.filter_by(barcode=barcode).count() is 0:
                book = Book()
                book.barcode = barcode
                book.name = arr[1]
                book.booktype_id = arr[2]
                book.author = arr[3]
                book.press = arr[4]
                bdate = datetime.strptime(arr[5],'%Y-%m-%d')
                book.date = bdate
                book.price = arr[6]
                book.page = arr[7]
                book.bookshelf = arr[8]
                book.allcount = arr[9]
                db.session.add(book)
                db.session.commit()
                return "true"
    return "false"

#获取图书完整信息
@app.route("/book/getcomplete",methods=["GET"])
def book_getcomplete():
    barcode = request.args.get("barcode")
    if barcode:
        book = Book.query.filter_by(barcode=barcode).first()
        if book:
            return json.dumps(book.toCompleteData())
    return "false"

#获取一个图书的信息
@app.route("/book/getone",methods=["GET"])
def book_getone():
    barcode = request.args.get("barcode")
    if barcode:
        book = Book.query.filter_by(barcode=barcode).first()
        if book:
            return json.dumps(book.toArrayData())
    return "false"

#删除图书
@app.route("/book/delete",methods=["GET"])
def book_delete():
    if tool.bookmanageValid(session):
        barcode = request.args.get("barcode")
        if barcode:
            book = Book.query.filter_by(barcode=barcode).first()
            if book and book.borrows.count() is 0:
                historys = book.historys.all()
                for data in historys:
                    db.session.delete (data)
                db.session.delete(book)
                db.session.commit()
                return "true"
    return "false"

#更改图书信息
@app.route("/book/change",methods=["POST"])
def book_change():
    if tool.bookmanageValid(session):
        data = request.form.get("data")
        if data:
            arr = json.loads(unquote(data))
            barcode = arr[10]
            book = Book.query.filter_by(barcode=barcode).first()
            if book and (arr[0]==arr[10] or Book.query.filter_by(barcode=arr[0]).count() is 0):

                book.barcode = arr[0]
                book.name = arr[1]
                book.booktype_id = arr[2]
                book.author = arr[3]
                book.press = arr[4]
                bdate = datetime.strptime(arr[5],'%Y-%m-%d')
                book.date = bdate
                book.price = arr[6]
                book.page = arr[7]
                book.bookshelf = arr[8]
                if arr[9] >= book.borrows.count():
                    book.allcount = arr[9]
                    db.session.add(book)
                    db.session.commit()
                    return "true"
    return "false"

#图书查询 0->书名,1->条形码,2->作者,3->出版社,4->书架
@app.route("/book/query",methods=["GET"])
def book_query():
    page = tool.strtoint(request.args.get("page"),0)
    query_type = tool.strtoint(request.args.get("type"),0)
    btype_id = tool.strtoint(request.args.get("btype"),-1)
    query_str = request.args.get("query")
    booklist = Book.query
    if btype_id is not -1:
        booklist = booklist.filter(Book.booktype_id==btype_id)
    if query_str:
        query_str = unquote(query_str)
        if query_type is 0:
            booklist = booklist.filter(Book.name.ilike("%"+query_str+"%"))
        elif query_type is 1:
            booklist = booklist.filter(Book.barcode==query_str)
        elif query_type is 2:
            booklist = booklist.filter(Book.author.ilike("%"+query_str+"%"))
        elif query_type is 3:
            booklist = booklist.filter(Book.press.ilike("%"+query_str+"%"))
        elif query_type is 4:
            booklist = booklist.filter(Book.bookshelf==tool.strtoint(query_str,0))
    booklist = booklist.offset(Book.pageCount*page).limit(Book.pageCount).all()
    return json.dumps(Book.getsome(booklist))

#图书管理查询 sort->0 默认排序 sort->1 出版时间排序 sort->2 借阅次数排序 reverse倒序
@app.route("/book/manage/query",methods=["GET"])
def book_manage_query():
    if tool.bookmanageValid(session):
        page = tool.strtoint(request.args.get("page"),0)
        sort = tool.strtoint(request.args.get("sort"),0)
        reverse = tool.strtoint(request.args.get("reverse"),0)
        query = request.args.get("query")
        if query:
            booklist = Book.query.filter_by(barcode=query).all()
        else:
            booklist = db.session.query(Book).outerjoin(History).group_by(Book)
            if reverse is 0:
                if sort is 0:
                    booklist = booklist.order_by(Book.barcode)
                elif sort is 1:
                    booklist = booklist.order_by(db.desc(Book.date))
                elif sort is 2:
                    booklist = booklist.order_by(db.desc(db.func.count(History.id)))
            else:
                if sort is 0:
                    booklist = booklist.order_by(db.desc(Book.barcode))
                elif sort is 1:
                    booklist = booklist.order_by(Book.date)
                elif sort is 2:
                    booklist = booklist.order_by(db.func.count(History.id))
                    
            booklist = booklist.offset(Book.pageCount*page).limit(Book.pageCount).all()
        return json.dumps(Book.getManageData(booklist))
    return "false"
