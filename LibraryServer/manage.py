from views import db,app
from flask.ext.script import Server, Manager, prompt_bool
from flask.ext.migrate import Migrate, MigrateCommand

migrate = Migrate(app, db)
manager = Manager(app)
manager.add_command('db', MigrateCommand)
manager.add_command("runserver", Server('0.0.0.0', port=5000))


@manager.command
def createall():
    db.create_all()

@manager.command
def dropall():
    if prompt_bool("Are you sure ? You will lose all your data !"):
        db.drop_all()

if __name__ == "__main__":
    app.run(debug=True)