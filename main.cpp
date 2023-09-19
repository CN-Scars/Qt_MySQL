#include "widget.h"
#include "logindialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    LoginDialog login;
    QObject::connect(&login, &LoginDialog::databaseConnected, &w, &Widget::handleDatabaseConnected);
    if (login.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
