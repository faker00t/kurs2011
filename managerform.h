#ifndef MANAGERFORM_H
#define MANAGERFORM_H

#include <QWidget>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QtSql/QSqlDatabase>

namespace Ui {
class ManagerForm;
}

class ManagerForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit ManagerForm(QWidget *parent = 0);
    ~ManagerForm();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::ManagerForm *ui;
public slots:
    void up();
};

#endif // MANAGERFORM_H
