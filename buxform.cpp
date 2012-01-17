#include "buxform.h"
#include "ui_buxform.h"

BuxForm::BuxForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuxForm)
{
    ui->setupUi(this);
}

BuxForm::~BuxForm()
{
    delete ui;
}

void BuxForm::on_pushButton_clicked() /// запросы бухгалтера
{
    ui->buxTable->setRowCount(0);
    QSqlQuery q;
    q.exec("select manager_id,name,count(manager_id) as c from bill inner join workers on manager_id = workers.id group by manager_id order by c desc");
    int row = 0;
    while (q.next())
    {
        ui->buxTable->insertRow(row);
        ui->buxTable->setItem(row,0,new QTableWidgetItem(q.value(1).toString()));
        ui->buxTable->setItem(row,1,new QTableWidgetItem(q.value(2).toString()));
        ++row;
    }
}

void BuxForm::up()
{
    on_pushButton_clicked();
}
