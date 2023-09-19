#include "logindialog.h"
#include "ui_logindialog.h"

#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)

{
    ui->setupUi(this);

    setWindowTitle("登录窗口");

    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->host_lineEdit->setText(DEFAULT_HOST);
    ui->port_lineEdit->setText(QString::number(DEFAULT_PORT));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_login_pushButton_clicked()
{
    // 连接数据库
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ui->host_lineEdit->text());
    db.setPort(ui->port_lineEdit->text().toInt());
    db.setDatabaseName(ui->databaseName_lineEdit->text());

    db.setUserName(ui->userName_lineEdit->text());
    db.setPassword(ui->password_lineEdit->text());

    if (db.open() and !ui->databaseName_lineEdit->text().isEmpty())
    {
        QMessageBox::information(this, "连接提示", "连接成功");
        accept();
        emit databaseConnected(db);
    }
    else
    {
        QMessageBox::warning(this, "连接提示", "连接失败，请确认信息是否正确！");
        qDebug() << db.lastError();
    }
}
