#include "formkassir.h"

FormKassir::FormKassir(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout  *h3 = new QHBoxLayout();
    QVBoxLayout *v = new QVBoxLayout();
    QHBoxLayout *h4 = new QHBoxLayout();
    QHBoxLayout *h5 = new QHBoxLayout();
    cancel = new QPushButton("Закрыть");
    payed = new QPushButton("Оплачено");
    l_bill = new QLabel("Накладная №");
    l_bill_val = new QLabel();
    msg = new QLabel();
    sum = new QLabel();
    bill = new QTextEdit();
    h3->addWidget(cancel);
    h3->addWidget(payed);
    h4->addWidget(l_bill);
    h4->addWidget(l_bill_val);
    h5->addWidget(msg);
    h5->addWidget(sum);
    v->addLayout(h4);
    v->addLayout(h3);
    v->addLayout(h5);
    v->addWidget(bill);
    setLayout(v);

    connect(cancel,SIGNAL(clicked()),this,SLOT(on_cancel_clicked()));
    connect(payed,SIGNAL(clicked()),this,SLOT(on_payed_clicked()));
    setWindowTitle("Кассир");
}

//void FormKassir::new_bill(QString id)
//{
//    qDebug() << "FormKassir::new_bill";
//    l_bill_val->setText(id);
//    QSqlQuery q;//(db);
//    QSqlQuery q2;//(db);
//    QString s;
//    q.exec("select * from goods_list where bill_id="+id);
//    double sum1 = 0;
//    bill->setText("***ЧЕК***");
//    bill->append("Кассир: ...");
//    QDateTime dt;
//    bill->append("Дата: "+dt.currentDateTime().toString());
//    int i = 1;
//    while (q.next())
//    {
//        bool res = q2.exec("select id,count,rprice,name from goods where id="+q.value(1).toString());
//        if (res) qDebug() << "ok";
//        else qDebug() << "err";
//        q2.next();
//        qDebug() << ">>> count = " << q2.value(1).toString() << " id = " << q.value(1).toString();
//        sum1 += q2.value(2).toDouble();
//        s.sprintf("%i",i);
//        bill->append(s+". "+q2.value(3).toString()+"   "+q2.value(2).toString());
//        ++i;
//    }

//    s.sprintf("%.2f",sum1);
//    sum->setText("Сумма "+s);
//    bill->append("Сумма   "+s);
//    bill->append("Спасибо за покупку!");

//}

void FormKassir::new_bill(QString id)
{
    qDebug() << "FormKassir::new_bill";
    l_bill_val->setText(id);
    QSqlQuery q;//(db);
    QSqlQuery q2;//(db);
    QString s;
    q.exec("select goods_id,name,rprice from goods_list inner join goods on goods_id = goods.id where bill_id="+id);
    double sum1 = 0;
    bill->setText("***ЧЕК***");
    bill->append("Кассир: ...");
    QDateTime dt;
    bill->append("Дата: "+dt.currentDateTime().toString());
    int i = 1;
    while (q.next())
    {
        //sum1 += q2.value(2).toDouble();
        s.sprintf("%i",i);
        bill->append(s+". "+q.value(1).toString()+"   "+q.value(2).toString());
        ++i;
    }
    bool res = q.exec("select goods_id,name,sum(rprice) as summ from goods_list inner join goods on goods_id = goods.id where bill_id="+id);
    if (res) qDebug() << "ok sum";
    else qDebug() << "err sum";
    s.sprintf("%.2f",sum1);
    q.next();
    sum->setText("Сумма "+q.value(2).toString());
    bill->append("Сумма   "+q.value(2).toString());
    bill->append("Спасибо за покупку!");

}

void FormKassir::on_cancel_clicked()
{
    hide();
}

void FormKassir::on_payed_clicked()
{
    QSqlQuery q;//(db);
    QSqlQuery q2;//(db);
    QString s,id(l_bill_val->text());
    q.exec("select payed from bill where id="+id);
    q.next();
    qDebug() << q.value(0).toInt();
    if (q.value(0).toInt() == 1)
    {
        msg->setText("Накладная уже оплачена");
        return;
    }
    q.exec("select * from goods_list where bill_id="+id);
    while (q.next())
    {
        bool res = q2.exec("select id,count from goods where id="+q.value(1).toString());
        if (res) qDebug() << "ok";
        else qDebug() << "err";
        q2.next();
        qDebug() << ">>> count = " << q2.value(1).toString() << " id = " << q.value(1).toString();
        int count = q2.value(1).toInt();
        if (count - 1 < 0) msg->setText("Недостаточно товаров на складе");
        else msg->setText("Товар оплачен");
        s.sprintf("%i",count-1);
        res = q2.exec("update goods set count="+s+" where id = "+q.value(1).toString());
    }
    q.exec("update bill set payed=1 where id="+id);

}

void FormKassir::get_db(QSqlDatabase & _db)
{
    //db = _db;
}
