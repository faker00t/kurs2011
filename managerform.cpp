#include "managerform.h"
#include "ui_managerform.h"

ManagerForm::ManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManagerForm)
{
    ui->setupUi(this);
}

ManagerForm::~ManagerForm()
{
    delete ui;
}

void ManagerForm::on_pushButton_clicked() /// запросы менеджера
{
    QSqlQuery q;
    ui->tableTopGoods->setRowCount(0);
    bool res = q.exec("select total,goods_id,goods.name,maker_id,makers.name from (select count(goods_id) as total,goods_id from goods_list group by goods_id) as tmp inner join goods on tmp.goods_id = goods.id inner join makers on maker_id = makers.id order by total desc");
    if (res) qDebug()<<"ok"; /// поиск самых заказываемых товаров
    else qDebug()<<"err";
    int row = 0;
    while (q.next())
    {
        ui->tableTopGoods->insertRow(row);
        ui->tableTopGoods->setItem(row,1,new QTableWidgetItem(q.value(0).toString()));
        ui->tableTopGoods->setItem(row,0,new QTableWidgetItem(q.value(2).toString()));
        ui->tableTopGoods->setItem(row,2,new QTableWidgetItem(q.value(4).toString()));
        ++row;
    }

    row = 0;
    ui->tableLowGoods->setRowCount(0);
    res = q.exec("select goods.name,count,maker_id,makers.name,phone from goods inner join makers on maker_id = makers.id where count < 3");
    while (q.next()) /// поиск заканчивающихся на складе товаров
    {
        ui->tableLowGoods->insertRow(row);
        ui->tableLowGoods->setItem(row,0,new QTableWidgetItem(q.value(0).toString()));
        ui->tableLowGoods->setItem(row,1,new QTableWidgetItem(q.value(1).toString()));
        ui->tableLowGoods->setItem(row,2,new QTableWidgetItem(q.value(3).toString()));
        ui->tableLowGoods->setItem(row,3,new QTableWidgetItem(q.value(4).toString()));
        ++row;
    }
}

void ManagerForm::up()
{
    on_pushButton_clicked();
}
