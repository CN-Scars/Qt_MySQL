 #include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("操作数据库表");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setDatabase(QSqlDatabase db)
{
    this->db = db;
}

void Widget::addFields(QStringList fields)
{
    ui->fields_comboBox->clear();
    ui->fields_comboBox->addItems(fields);
}

void Widget::addTables(QStringList tables)
{
    ui->tables_comboBox->clear();
    ui->tables_comboBox->addItems(tables);
}

QString Widget::getFieldAndProperties(QString fieldName)
{
    QString properties;

    // 检查该字段是否存在于columnInfos中
    if (columnInfos.contains(fieldName))
    {
        QMap<QString, QString> fieldInfo = columnInfos[fieldName];

        // 组织输出
        properties += QString("字段名称: %1\n").arg(fieldName);

        QMapIterator<QString, QString> it(fieldInfo);
        while (it.hasNext())
        {
            it.next();
            properties += QString("%1: %2\n").arg(it.key()).arg(it.value());
        }
    }
    else
    {
        properties = "字段未找到！";
    }

    return properties;
}

void Widget::getTables()
{
    tables.clear();

    QSqlQuery query;

    if (query.exec("SHOW TABLES"))
    {
        while (query.next())
        {
            tables << query.value(0).toString();
        }
    }
}

void Widget::on_Database_tableView_clicked(const QModelIndex &index)
{
    int currentRow = index.row();
    selectedRow = currentRow;

    QString fieldName = model->headerData(index.column(), Qt::Horizontal).toString();
    ui->columnInfos_textBrowser->setText(getFieldAndProperties(fieldName));
}

void Widget::on_queryButton_clicked()
{
    QString field = ui->fields_comboBox->currentText();
    QString value = ui->valueEditor_lineEdit->text();
    QString filter = QString("%1 LIKE '%%2%'").arg(field).arg(value);

    model->setFilter(filter);

    model->select();
}

void Widget::on_insertButton_clicked()
{
    int newRow = model->rowCount();

    if (!model->insertRow(newRow))
    {
        QString errorMessage = model->lastError().text();
        QMessageBox::critical(this, "无法插入数据", "插入新行时出现错误，错误信息为：\n" + errorMessage);
    }
}

void Widget::on_deleteButton_clicked()
{
    if (selectedRow == DEFAULTROW)
    {
        QMessageBox::warning(this, "删除错误", "未选中要删除的行!");
        return;
    }

    model->removeRow(selectedRow);

    if (!model->submitAll())
    {
        QString errorMessage = model->lastError().text();
        QMessageBox::critical(this, "删除错误", "未能成功删除数据，错误信息为：\n" + errorMessage);
        model->revertAll();
    }

    model->select();
    selectedRow = DEFAULTROW;

    ui->valueEditor_lineEdit->clear();
}

void Widget::handleDatabaseConnected(QSqlDatabase db)
{
    setDatabase(db);

    model = new QSqlTableModel(this, db);

    getTables();

    addTables(tables);
}

void Widget::setCurrentTable(QString tableName)
{
    model->setTable(tableName);
    model->select();
    ui->Database_tableView->setModel(model);

    showColumns(tableName);
}

void Widget::showColumns(QString tableName)
{
    model->setTable(tableName);
    model->select();
    ui->Database_tableView->setModel(model);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    fields.clear();

    QSqlQuery query;
    if (query.exec(QString("show columns from %1").arg(tableName)))
    {
        QSqlRecord record = query.record();
        while (query.next())
        {
            for (int i = 1; i < record.count(); ++i)
            {
                columnInfos[query.value(0).toString()][record.fieldName(i)] = query.value(i).toString();
            }
            fields << query.value(0).toString();
        }
        addFields(fields);
    }
    else
    {
        QString errorMessage = model->lastError().text();
        QMessageBox::critical(this, "获取信息错误", "无法完成表数据的获取！错误信息为：\n" + errorMessage);
    }
}

void Widget::on_tables_comboBox_currentIndexChanged(int index)
{
    QString tableName = ui->tables_comboBox->currentText();

    model->setTable(tableName);
    model->select();
    ui->Database_tableView->setModel(model);

    showColumns(tableName);
}

void Widget::on_submitNewRowButton_clicked()
{
    if (!model->submitAll())
    {
        QString errorMessage = model->lastError().text();
        QMessageBox::critical(this, "无法保存数据", "提交更改发生了错误，错误信息为：\n" + errorMessage);
        model->revertAll();
    }
}
