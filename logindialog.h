#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_login_pushButton_clicked();

private:
    Ui::LoginDialog *ui;
    QSqlDatabase db;
    const QString DEFAULT_HOST = "localhost";
    static const int DEFAULT_PORT = 3306;

signals:
    void databaseConnected(QSqlDatabase db);
};

#endif // LOGINDIALOG_H
