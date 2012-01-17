#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QLabel>
#include <QTableWidgetItem>
#include <QDebug>
#include <QComboBox>
#include "formusers.h"
#include <QCryptographicHash>
#include "formkassir.h"
#include "managerform.h"
#include "buxform.h"
#include "forminstall.h"
#include <QMessageBox>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static bool createConnection();

private slots:
    void on_tabBill_cellChanged(int row, int column);
    void on_CreateDBButton_clicked();

    void on_ShowButton_clicked();

    void on_InsertButton_clicked();

    void on_tab_cellChanged(int row, int column);

    void on_tab_cellClicked(int row, int column);

    void on_addMakerButton_clicked();

    void on_showMakerButton_clicked();

    void on_addTypeButton_clicked();

    void on_showTypeButton_clicked();

    void on_tabMaker_cellClicked(int row, int column);

    void on_tabMaker_cellChanged(int row, int column);

    void on_tabType_cellClicked(int row, int column);

    void on_tabType_cellChanged(int row, int column);

    void on_Cb_changed(int);

    void on_CbType_changed(int);
    void on_billButton_clicked();

    void on_makersButton_clicked();

    void on_spoilerButton_clicked();

    void on_custoersButton_clicked();

    void on_createBillButton_clicked();

    void on_showBillButton_clicked();

    void on_showListButton_clicked();

    void on_tabBill_cellClicked(int, int);

    void on_addListButton_clicked();

    void on_comboBoxUser_currentIndexChanged(int);

    void on_showCustButton_clicked();

    void on_addCustButton_clicked();

    void on_tableCust_cellClicked(int row, int column);

    void on_tableCust_cellChanged(int row, int column);

    void on_CbBillSup_changed(int);

    void on_userButton_clicked();

    void on_CbBillCust_changed(int);

    void on_workersButton_clicked();

    void on_showWorkersButton_clicked();

public slots:
    void slot_users(QString,QString);
private:
    Ui::MainWindow *ui;
    QSqlQuery query;
    //QSqlDatabase db;
    QLabel *l;
    bool res;
    int cellEnteredRow;
    int cellEnteredMakerRow;
    int cellEnteredTypeRow;
    bool can_add;
    int cellEnteredBillRow;
    int cellEnteredCustRow;
    FormUsers *users;
    FormKassir *kass;
    ManagerForm *manform;
    BuxForm *bux;
    FormInstall *install;
signals:
    void select_bill(QString);
    void send_db(QSqlDatabase & );
    void managerFormUp();
    void buxFormUp();
};

#endif // MAINWINDOW_H
