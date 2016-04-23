from models import Adminer

#str转int，带默认值
def strtoint(s, default):
    try:
        s = int(s)
    except ValueError and TypeError:
        s = default
    return s

#判断图书管理合法性
def bookmanageValid(session):
    logintype = session.get("logintype")
    if logintype:
        logintype = strtoint(logintype, -1)
        if logintype is 2 or \
         (logintype is 1 and Adminer.query.filter_by(username=session.get("username")).first().bookmanage is 1):
            return True
    return False

#判断图书管理合法性
def readermanageValid(session):
    logintype = session.get("logintype")
    if logintype:
        logintype = strtoint(logintype, -1)
        if logintype is 2 or \
         (logintype is 1 and Adminer.query.filter_by(username=session.get("username")).first().readermanage is 1):
            return True
    return False
