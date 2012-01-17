#include "formusers.h"

FormUsers::FormUsers(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout  *h1 = new QHBoxLayout();
    QHBoxLayout  *h2 = new QHBoxLayout();
    QHBoxLayout  *h3 = new QHBoxLayout();
    QVBoxLayout *v = new QVBoxLayout();
    l_user = new QLabel("Имя пользователя");
    user = new QLineEdit();
    l_pass = new QLabel("Пароль");
    pass = new QLineEdit();
    pass->setEchoMode(QLineEdit::Password);
    ok = new QPushButton("Войти");
    cancel = new QPushButton("Закрыть");
    h1->addWidget(l_user);
    h1->addWidget(user);
    h2->addWidget(l_pass);
    h2->addWidget(pass);
    h3->addWidget(ok);
    h3->addWidget(cancel);
    v->addLayout(h1);
    v->addLayout(h2);
    v->addLayout(h3);
    setLayout(v);
    qDebug()<<"FormUsers::FormUsers()";
    connect(ok,SIGNAL(clicked()),this,SLOT(on_ok_clicked()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(on_cancel_clicked()));
    setWindowTitle("Вход в систему");
}

FormUsers::~FormUsers()
{
    qDebug()<<"FormUsers::~FormUsers()";
}

void FormUsers::on_ok_clicked()
{
    emit send(user->text(),pass->text());
    hide();
    user->clear();
    pass->clear();
}
void FormUsers::on_cancel_clicked()
{
    hide();
}
