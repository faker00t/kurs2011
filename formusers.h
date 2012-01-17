#ifndef FORMUSERS_H
#define FORMUSERS_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QString>

class FormUsers : public QWidget
{
    Q_OBJECT
public:
    explicit FormUsers(QWidget *parent = 0);
    ~FormUsers();
signals:
    void send(QString, QString);
public slots:
    void on_ok_clicked();
    void on_cancel_clicked();
private:
    QLabel *l_user;
    QLineEdit *user;
    QLabel *l_pass;
    QLineEdit *pass;
    QPushButton *ok;
    QPushButton *cancel;
};

#endif // FORMUSERS_H
