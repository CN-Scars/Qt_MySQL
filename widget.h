#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setDatabase(QSqlDatabase db);

    void addFields(QStringList fields);

    void addTables(QStringList tables);

    void getTables();

    QString getFieldAndProperties(QString fieldName);

public slots:
    void handleDatabaseConnected(QSqlDatabase db);
    void setCurrentTable(QString tableName);

private slots:
    void on_queryButton_clicked();

    void on_insertButton_clicked();

    void on_Database_tableView_clicked(const QModelIndex &index);

    void on_deleteButton_clicked();

    void showColumns(QString tableName);

    void on_tables_comboBox_currentIndexChanged(int index);

    void on_submitNewRowButton_clicked();

private:
    Ui::Widget *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    static const int DEFAULTROW = -1;
    int selectedRow = DEFAULTROW;
    QMap<QString, QMap<QString, QString>> columnInfos;
    QStringList fields;
    QStringList tables;

};
#endif // WIDGET_H
