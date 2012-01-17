#include "forminstall.h"
#include "ui_forminstall.h"

FormInstall::FormInstall(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormInstall)
{
    ui->setupUi(this);
    setWindowTitle("Установка");
}

FormInstall::~FormInstall()
{
    delete ui;
}

void FormInstall::on_pushButton_clicked()
{
    bool res;
    QSqlQuery query;
    res = query.exec("create table makers (id integer not null primary key, name text, addr text, phone text);");
    res = query.exec("create table goods_type (id integer not null primary key, type text)");
    res = query.exec("create table goods (id integer not null primary key, name text, maker_id integer, pprice double, wprice double, rprice double, count integer, type_id integer, foreign key(maker_id) references makers(id), foreign key(type_id) references goods_type(id));");
    res = query.exec("create table clients (id integer not null primary key, name text, addr text, phone text, type integer)");
    res = query.exec("create table workers (id integer not null primary key, name text, addr text, phone text, user text unique, pass text, type integer)");
    res = query.exec("create table bill (id integer not null primary key, client_id integer, manager_id integer, date text, payed integer, foreign key(client_id) references clients(id), foreign key(manager_id) references workers(id))");
    res = query.exec("create table goods_list (bill_id integer, goods_id integer, foreign key(bill_id) references bill(id), foreign key(goods_id) references goods(id))");
//    res = query.exec(tr("create table users (user text primary key, pass text, type int)"));
//    res = query.exec("create index goods_id_index on goods (id)");
    res = query.exec("insert into workers (name,addr,phone,user,pass,type) values ('nick n ru','rostov','+7..','nick','4eae35f1b35977a00ebd8086c259d4c9',0)");
    res = query.exec("insert into workers (name,addr,phone,user,pass,type) values ('petya 1','rostov','+7..','1','c4ca4238a0b923820dcc509a6f75849b',1)");
    res = query.exec("insert into workers (name,addr,phone,user,pass,type) values ('vasya 2','rostov','+7..','2','c81e728d9d4c2f636f067f89cc14862c',2)");
    res = query.exec("insert into workers (name,addr,phone,user,pass,type) values ('fedya 3','rostov','+7..','3','eccbc87e4b5ce2fe28308fd9f2a7baf3',3)");
    res = query.exec("create table info (i text)");
    ui->textEdit->setText("Начало установки...");
    ui->textEdit->append("Таблицы созданы...");
    res = query.exec("insert into info (i) values ('"+ui->lineEdit->text()+"')");
    res = query.exec("insert into info (i) values ('"+ui->lineEdit_2->text()+"')");
    res = query.exec("insert into info (i) values ('"+ui->lineEdit_3->text()+"')");
    ui->textEdit->append("Информация об организации добавлена...");
    ui->textEdit->append("Установка завершена.");
//    4eae35f1b35977a00ebd8086c259d4c9
//    c4ca4238a0b923820dcc509a6f75849b
//    c81e728d9d4c2f636f067f89cc14862c
//    eccbc87e4b5ce2fe28308fd9f2a7baf3
}
