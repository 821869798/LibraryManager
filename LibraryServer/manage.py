from views import db,app
from flask.ext.script import Server, Manager, prompt_bool
from flask.ext.migrate import Migrate, MigrateCommand

manager = Manager(app)
migrate = Migrate(app, db)
manager.add_command('db', MigrateCommand)
manager.add_command("runserver", Server('0.0.0.0', port=5000))


@manager.command
def createall():
    db.create_all()
    f = open("static/默认数据.txt","r",encoding="utf-8")
    for line in f.readlines():
        if line:
            db.session.execute(line)
    f.close()
    db.session.commit()

@manager.command
def dropall():
    if prompt_bool("Are you sure ? You will lose all your data !"):
        db.drop_all()

if __name__ == "__main__":
    manager.run()

# if __name__ == "__main__":
#     app.run(debug=True)