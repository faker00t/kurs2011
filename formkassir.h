#ifndef FORMKASSIR_H
#define FORMKASSIR_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QTextEdit>
#include <QDateTime>

class FormKassir : public QWidget
{
    Q_OBJECT
public:
    explicit FormKassir(QWidget *parent = 0);

signals:

public slots:
    void new_bill(QString);
    void on_cancel_clicked();
    void on_payed_clicked();
    void get_db(QSqlDatabase & );
private:
    QPushButton *cancel;
    QPushButton *payed;
    QLabel *l_bill;
    QLabel *l_bill_val;
    //QSqlDatabase db;
    QLabel *msg;
    QLabel *sum;
    QTextEdit *bill;
};

#endif // FORMKASSIR_H
