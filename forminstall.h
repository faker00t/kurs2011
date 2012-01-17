#ifndef FORMINSTALL_H
#define FORMINSTALL_H

#include <QWidget>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QtSql/QSqlDatabase>

namespace Ui {
class FormInstall;
}

class FormInstall : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormInstall(QWidget *parent = 0);
    ~FormInstall();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::FormInstall *ui;
};

#endif // FORMINSTALL_H
