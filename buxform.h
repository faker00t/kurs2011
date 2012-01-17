#ifndef BUXFORM_H
#define BUXFORM_H

#include <QWidget>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QtSql/QSqlDatabase>

namespace Ui {
class BuxForm;
}

class BuxForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit BuxForm(QWidget *parent = 0);
    ~BuxForm();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::BuxForm *ui;
public slots:
    void up();
};

#endif // BUXFORM_H
