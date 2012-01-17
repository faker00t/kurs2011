#include "mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::createConnection() /// создание подключения к БД
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); /// драйвер БД
    db.setDatabaseName("../irma.db"); /// имя файла БД
    if (!db.open()) {
        return false;
    }
    return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bux = 0;
    l = new QLabel;
    ui->statusBar->addWidget(l);
    if (query.exec("select * from info"))
    {
        l->setText("opened");
        query.next();
        setWindowTitle("База данных склада \""+query.value(0).toString()+"\"");
    }

    else {
        l->setText("open fail");
        install = new FormInstall();
        install->show();
    }
    res = query.exec("PRAGMA foreign_keys = ON");
    cellEnteredRow = -1;
    can_add = false;
    on_ShowButton_clicked();
    on_showMakerButton_clicked();
    on_showTypeButton_clicked();
    on_spoilerButton_clicked();
    on_showCustButton_clicked();
    on_showWorkersButton_clicked();
    ui->comboBoxUser->addItem("Администратор"); // 0
    ui->comboBoxUser->addItem("Менеджер"); // 1
    ui->comboBoxUser->addItem("Кассир"); // 2
    ui->comboBoxUser->addItem("Работник склада"); // 3
    ui->comboBoxUser->addItem("Никто"); // 4
    ui->comboBoxUser->setCurrentIndex(4);
    ui->comboBoxUser->setDisabled(true);

    ui->makersButton->setDisabled(true);
    ui->custoersButton->setDisabled(true);
    ui->billButton->setDisabled(true);
    ui->InsertButton->setDisabled(true);
    //ui->CreateDBButton->setDisabled(true);
    ui->tab->setDisabled(true);
    ui->workersButton->setDisabled(true);

    users = new FormUsers();
    connect(users,SIGNAL(send(QString,QString)),this,SLOT(slot_users(QString,QString)));
    kass = new FormKassir();
    connect(this,SIGNAL(select_bill(QString)),kass,SLOT(new_bill(QString)));
    connect(this,SIGNAL(send_db(QSqlDatabase&)),kass,SLOT(get_db(QSqlDatabase&)));
    manform = new ManagerForm();
    connect(this,SIGNAL(managerFormUp()),manform,SLOT(up()));
    bux = new BuxForm();
    connect(this,SIGNAL(buxFormUp()),bux,SLOT(up()));
    //emit send_db(db);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CreateDBButton_clicked()
{
    query.exec("select * from info");
    query.next();
    QString s1,s2,s3;
    s1 = query.value(0).toString();
    query.next();
    s2 = query.value(0).toString();
    query.next();
    s3 = query.value(0).toString();
    QMessageBox::warning(this, tr("Информация"),
                   "Название: "+s1+"\n"+
                      "Телефон: "+s2+"\n"+
                      "Директор: "+s3+"\n",
                   QMessageBox::Ok);
}

void MainWindow::on_ShowButton_clicked()
{
    ui->tab->setRowCount(0);
    res = query.exec("select * from goods");
    if (res)  l->setText("select ok");
    else l->setText("select error");
    int row = 0;
    QTableWidgetItem *item;
    while (query.next()) {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        qDebug() << id << ' ' << name;
        ui->tab->insertRow(row);
        (item = new QTableWidgetItem(id))->setFlags(Qt::NoItemFlags);
        ui->tab->setItem(row,0,item);
        ui->tab->setItem(row,1,new QTableWidgetItem(name));
        //QSqlQuery q(db),q_maker(db);
        QSqlQuery q,q_maker;
        q_maker.exec("select name from makers where id = " + query.value(2).toString());
        q_maker.next();
        QString maker = q_maker.value(0).toString();
        q.exec("select name from makers");
        //ui->tab->setItem(row,2,new QTableWidgetItem( query.value(2).toString() ) );
        QComboBox *cb = new QComboBox();
        int i = 0;
        while (q.next())
        {
            cb->addItem(q.value(0).toString());
            if (q.value(0).toString() == maker) cb->setCurrentIndex(i);
            ++i;
        }

        ui->tab->setCellWidget(row,2,cb);
        connect(qobject_cast<QComboBox*>(ui->tab->cellWidget(row, 2)),SIGNAL(currentIndexChanged(int)),this,SLOT(on_Cb_changed(int)));
        //ui->tab->setItem(row,2,new QTableWidgetItem( item ) );
        //ui->tab->setItem(row,2,new QTableWidgetItem( q.value(0).toString() ) );
        ui->tab->setItem(row,3,new QTableWidgetItem( query.value(3).toString() ) );
        ui->tab->setItem(row,4,new QTableWidgetItem( query.value(4).toString() ) );
        //ui->tab->setItem(row,5,new QTableWidgetItem( query.value(5).toString() ) );
        ui->tab->setItem(row,6,new QTableWidgetItem( query.value(6).toString() ) );
        item = new QTableWidgetItem(query.value(5).toString());
        ui->tab->setItem(row,5,item);
        //ui->tab->openPersistentEditor(item);
        q.exec("select type from goods_type where id = " + query.value(7).toString());
        q.next();
        //ui->tab->setItem(row,7,new QTableWidgetItem( query.value(7).toString() ) );
        //ui->tab->setItem(row,7,new QTableWidgetItem( q.value(0).toString() ) );
        q.exec("select type from goods_type");
        //QSqlQuery q_type(db);
        QSqlQuery q_type;
        q_type.exec("select type from goods_type where id = \'" + query.value(7).toString()+"\'");
        q_type.next();
        qDebug() << q_type.value(0).toString();
        cb = new QComboBox();
        i = 0;
        while (q.next())
        {
            cb->addItem(q.value(0).toString());
            if (q.value(0).toString() == q_type.value(0).toString()) cb->setCurrentIndex(i);
            ++i;
        }
        ui->tab->setCellWidget(row,7,cb);
        connect(qobject_cast<QComboBox*>(ui->tab->cellWidget(row, 7)),SIGNAL(currentIndexChanged(int)),this,SLOT(on_CbType_changed(int)));
        ++row;
    }
}

void MainWindow::on_Cb_changed(int index)
{
    qDebug() << "MainWindow::on_Cb_changed(int index)" << index;
    qDebug() << ui->tab->currentRow();
    cellEnteredRow = ui->tab->currentRow();
//    int rows = ui->tab->rowCount();
//    for (int i = 0; i < rows; ++i)
//    {
//        if (this == qobject_cast<QComboBox*>(ui->tab->cellWidget(i, 2))) qDebug("yes!");
//        else qDebug("no!");
//    }
    //
    if (cellEnteredRow == -1)
    {
        l->setText("select row!");
        return;
    }
    on_tab_cellChanged(ui->tab->currentRow(),2);
}

void MainWindow::on_CbType_changed(int index)
{
    qDebug() << index;
    qDebug() << ui->tab->currentRow();
    cellEnteredRow = ui->tab->currentRow();
    if (cellEnteredRow == -1)
    {
        l->setText("select row!");
        return;
    }
    on_tab_cellChanged(ui->tab->currentRow(),7);
}

void MainWindow::on_InsertButton_clicked() /// вставка нового товара в таблицу
{
    res = query.exec("select id from goods order by id desc limit 1");
    if (res) l->setText("insert select ok");
    else l->setText("insert select error");
    query.next();
    int newid = query.value(0).toInt() + 1;
    qDebug() << newid;
    QString s;
    s.sprintf("%i",newid);
    qDebug() << s;
    res = query.exec("insert into goods (id,name,maker_id,pprice,wprice,rprice,count,type_id) values ("+s+",'name',1,0,0,0,0,1)");
    if (res) qDebug("insert ok");
    else qDebug("insert error");

    if (res) l->setText("insert ok");
    else l->setText("insert error");
    on_ShowButton_clicked();
}

void MainWindow::on_tab_cellChanged(int row, int column)
{
    if (row != cellEnteredRow) return;
    cellEnteredRow = -1;
    QTableWidgetItem *item = ui->tab->item(row,0);
    int id = item->text().toInt();
    qDebug() << "id = " << id;
    item = ui->tab->item(row,column);
    QString s;
    s.sprintf("%i",id);
    switch (column)
    {
    case 0: return;
    case 1: res = query.exec("update goods set name=\'"+item->text()+"\' where id="+s); break;
    case 2: {
        qDebug() << row << " x " << column;
        QComboBox* cb(qobject_cast<QComboBox*>(ui->tab->cellWidget(row, column)));
        QSqlQuery q;//(db);
        qDebug() << cb->currentText();
        q.exec("select id from makers where name = \'"+cb->currentText()+"\'");
        q.next();
        QString maker(q.value(0).toString());
        res = query.exec("update goods set maker_id="+maker+" where id="+s); break;
    }
    case 3: res = query.exec("update goods set pprice="+item->text()+" where id="+s); break;
    case 4: res = query.exec("update goods set wprice="+item->text()+" where id="+s); break;
    case 5: res = query.exec("update goods set rprice="+item->text()+" where id="+s); break;
    case 6: res = query.exec("update goods set count="+item->text()+" where id="+s); break;
    case 7: {
        qDebug() << row << " x " << column;
        QComboBox* cb(qobject_cast<QComboBox*>(ui->tab->cellWidget(row, column)));
        QSqlQuery q;//(db);
        qDebug() << cb->currentText();
        q.exec("select id from goods_type where type = \'"+cb->currentText()+"\'");
        q.next();
        QString type(q.value(0).toString());
        res = query.exec("update goods set type_id="+type+" where id="+s); break;
    }
    default: return;
    }
}

void MainWindow::on_tab_cellClicked(int row, int /*column*/)
{
    qDebug("on_tab_cellClicked(int row, int column)");
    cellEnteredRow = row;
    if (ui->bills->isVisible() && can_add)
    {
        qDebug() << "=))))))))))";
        int bill = ui->tabBill->currentRow()+1;
        if (bill < 1) return;
        QString s;
        s.sprintf("%i",bill);
        res = query.exec(tr("insert into goods_list (bill_id, goods_id) values (")+s+","+ui->tab->item(row,0)->text()+")");
        qDebug() << res << " " << bill;
        on_showListButton_clicked();
    }
}

void MainWindow::on_addMakerButton_clicked()
{
    res = query.exec("select id from makers order by id desc limit 1");
    if (res) l->setText("insert select ok");
    else l->setText("insert select error");
    query.next();
    int newid = query.value(0).toInt() + 1;
    qDebug() << newid;
    QString s;
    s.sprintf("%i",newid);
    qDebug() << s;
    res = query.exec("insert into makers (id,name,addr,phone) values ("+s+", 'name', 'addr', '+7()' )");
    if (res) qDebug("insert ok");
    else qDebug("insert fail");
    on_showMakerButton_clicked();
}

void MainWindow::on_showMakerButton_clicked()
{
    ui->tabMaker->setRowCount(0);
    res = query.exec("select * from makers");
    if (res)  l->setText("select ok");
    else l->setText("select error");
    int row = 0;
    QTableWidgetItem *item;
    while (query.next()) {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        qDebug() << id << ' ' << name;
        ui->tabMaker->insertRow(row);
        (item = new QTableWidgetItem(id))->setFlags(Qt::NoItemFlags);
        ui->tabMaker->setItem(row,0,item);
        ui->tabMaker->setItem(row,1,new QTableWidgetItem(name));
        ui->tabMaker->setItem(row,2,new QTableWidgetItem( query.value(2).toString() ) );
        ui->tabMaker->setItem(row,3,new QTableWidgetItem( query.value(3).toString() ) );
        ++row;
    }
}

void MainWindow::on_addTypeButton_clicked()
{
    res = query.exec("select id from goods_type order by id desc limit 1");
    if (res) l->setText("insert select ok");
    else l->setText("insert select error");
    query.next();
    int newid = query.value(0).toInt() + 1;
    qDebug() << newid;
    QString s;
    s.sprintf("%i",newid);
    qDebug() << s;
    res = query.exec("insert into goods_type (id,type) values ("+s+", 'type' )");
    if (res) qDebug("insert ok");
    else qDebug("insert fail");
    on_showTypeButton_clicked();
}

void MainWindow::on_showTypeButton_clicked()
{
    ui->tabType->setRowCount(0);
    res = query.exec("select * from goods_type");
    if (res)  l->setText("select ok");
    else l->setText("select error");
    int row = 0;
    QTableWidgetItem *item;
    while (query.next()) {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        qDebug() << id << ' ' << name;
        ui->tabType->insertRow(row);
        (item = new QTableWidgetItem(id))->setFlags(Qt::NoItemFlags);
        ui->tabType->setItem(row,0,item);
        ui->tabType->setItem(row,1,new QTableWidgetItem(name));
        ++row;
    }
}

void MainWindow::on_tabMaker_cellClicked(int row, int /*column*/)
{
    cellEnteredMakerRow = row;
}

void MainWindow::on_tabMaker_cellChanged(int row, int column)
{
    if (row != cellEnteredMakerRow) return;
    cellEnteredMakerRow = -1;
    QTableWidgetItem *item = ui->tabMaker->item(row,0);
    int id = item->text().toInt();
    qDebug() << "id = " << id;
    item = ui->tabMaker->item(row,column);
    QString s;
    s.sprintf("%i",id);
    switch (column)
    {
    case 0: return;
    case 1: res = query.exec("update makers set name=\'"+item->text()+"\' where id="+s); break;
    case 2: res = query.exec("update makers set addr=\'"+item->text()+"\' where id="+s); break;
    case 3: res = query.exec("update makers set phone=\'"+item->text()+"\' where id="+s); break;
    default: return;
    }
}

void MainWindow::on_tabType_cellClicked(int row, int /*column*/)
{
    cellEnteredTypeRow = row;
}

void MainWindow::on_tabType_cellChanged(int row, int column)
{
    if (row != cellEnteredTypeRow) return;
    cellEnteredTypeRow = -1;
    QTableWidgetItem *item = ui->tabType->item(row,0);
    int id = item->text().toInt();
    qDebug() << "id = " << id;
    item = ui->tabType->item(row,column);
    QString s;
    s.sprintf("%i",id);
    switch (column)
    {
    case 0: return;
    case 1: res = query.exec("update goods_type set type=\'"+item->text()+"\' where id="+s); break;
    default: return;
    }
}

void MainWindow::on_billButton_clicked()
{
    ui->bills->show();
    ui->customers->hide();
    ui->makers->hide();
    ui->workers->hide();
    on_showBillButton_clicked();
}

void MainWindow::on_makersButton_clicked()
{
    ui->bills->hide();
    ui->customers->hide();
    ui->workers->hide();
    ui->makers->show();
}

void MainWindow::on_spoilerButton_clicked()
{
    ui->bills->hide();
    ui->makers->hide();
    ui->customers->hide();
    ui->workers->hide();
}

void MainWindow::on_custoersButton_clicked()
{
    ui->bills->hide();
    ui->makers->hide();
    ui->workers->hide();
    ui->customers->show();
}

void MainWindow::on_createBillButton_clicked()
{
    res = query.exec("select id from bill order by id desc limit 1");
    if (res) l->setText("insert select ok");
    else l->setText("insert select error");
    query.next();
    int newid = query.value(0).toInt() + 1;
    qDebug() << "new bill "<<newid;
    QString s;
    s.sprintf("%i",newid);
    qDebug() << s;
    res = query.exec("insert into bill (id,client_id, manager_id, date, payed) values ("+s+",1,1,'2011-11-22',0 )");
    if (res) qDebug("insert ok");
    else qDebug("insert fail");
    on_showBillButton_clicked();
}

void MainWindow::on_showBillButton_clicked()
{
    ui->tabBill->setRowCount(0);
    res = query.exec("select * from bill");
    if (res)  l->setText("select ok");
    else l->setText("select error");
    int row = 0;
    QTableWidgetItem *item;
    while (query.next()) {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        qDebug() << id << ' ' << name;
        ui->tabBill->insertRow(row);
        (item = new QTableWidgetItem(id))->setFlags(Qt::NoItemFlags);
        ui->tabBill->setItem(row,0,item);
        ui->tabBill->setItem(row,1,new QTableWidgetItem(name));
        //ui->tabBill->setItem(row,2,new QTableWidgetItem( query.value(2).toString() ) );
        QSqlQuery q;//(db);
        q.exec("select id,name from clients");
        QComboBox *cb = new QComboBox();
        int i = 0;
        while (q.next())
        {
            cb->addItem(q.value(1).toString());
            if (q.value(0).toString() == query.value(1).toString()) cb->setCurrentIndex(i);
            qDebug() << "---- "<< q.value(0).toString() << ' ' << query.value(1).toString();
            ++i;
        }
        ui->tabBill->setCellWidget(row,1,cb);
        connect(qobject_cast<QComboBox*>(ui->tabBill->cellWidget(row, 1)),SIGNAL(currentIndexChanged(int)),this,SLOT(on_CbBillSup_changed(int)));

        cb = new QComboBox();
        i = 0;
        q.exec("select id,name from workers");
        while (q.next())
        {
            cb->addItem(q.value(1).toString());
            if (q.value(0).toString() == query.value(1).toString()) cb->setCurrentIndex(i);
            qDebug() << "---- "<< q.value(0).toString() << ' ' << query.value(1).toString();
            ++i;
        }
        ui->tabBill->setCellWidget(row,2,cb);
        connect(qobject_cast<QComboBox*>(ui->tabBill->cellWidget(row, 2)),SIGNAL(currentIndexChanged(int)),this,SLOT(on_CbBillCust_changed(int)));
        ui->tabBill->setItem(row,3,new QTableWidgetItem( query.value(3).toString() ) );
        ui->tabBill->setItem(row,4,new QTableWidgetItem( query.value(4).toString() ) );

        ++row;
    }
}

void MainWindow::on_CbBillSup_changed(int index)
{
    qDebug() << "on_CbBillSup_changed "<< index;
    qDebug() << ui->tabBill->currentRow();
    cellEnteredBillRow = ui->tabBill->currentRow();
    if (cellEnteredBillRow == -1)
    {
        l->setText("select row!");
        return;
    }
    on_tabBill_cellChanged(ui->tabBill->currentRow(),1);
}

void MainWindow::on_CbBillCust_changed(int index)
{
    qDebug() << "on_CbBillSup_changed "<< index;
    qDebug() << ui->tabBill->currentRow();
    cellEnteredBillRow = ui->tabBill->currentRow();
    if (cellEnteredBillRow == -1)
    {
        l->setText("select row!");
        return;
    }
    on_tabBill_cellChanged(ui->tabBill->currentRow(),2);
}

void MainWindow::on_showListButton_clicked()
{
    QString s;
    QSqlQuery q;//(db);
    int bill = ui->tabBill->currentRow()+1;
    s.sprintf("%i",bill);
    query.exec("select * from goods_list where bill_id="+s);
    int row = 0;
    ui->tableList->setRowCount(0);
    double sum = 0;
    while (query.next())
    {
        ui->tableList->insertRow(row);
        ui->tableList->setItem(row,0,new QTableWidgetItem( query.value(0).toString() ) );
        q.exec("select name from goods where id="+query.value(1).toString());
        q.next();
        ui->tableList->setItem(row,1,new QTableWidgetItem( q.value(0).toString() ) );
        q.exec("select rprice from goods where id="+query.value(1).toString());
        q.next();
        sum += q.value(0).toDouble();
        ++row;
    }
    emit select_bill(s);
    s.sprintf("%.2f",sum);
    ui->labelSum->setText(s);
    s.sprintf("%i",row);
    ui->labelTotal->setText(s);
}

void MainWindow::on_tabBill_cellClicked(int row, int /*column*/)
{
    on_showListButton_clicked();
    cellEnteredBillRow = row;
    qDebug() << "tabBillRow = "<< row;
}

void MainWindow::on_addListButton_clicked()
{
    if (can_add) {
        can_add = false;
        ui->addListButton->setText(tr("Добавить"));
    }
    else {
        can_add = true;
        ui->addListButton->setText(tr("Отменить добавление"));
    }
}

void MainWindow::on_comboBoxUser_currentIndexChanged(int index)
{
    //ui->CreateDBButton->setDisabled(true);
    on_spoilerButton_clicked();
    switch (index)
    {
    case 0: ui->makersButton->setDisabled(false);
        ui->custoersButton->setDisabled(false);
        ui->billButton->setDisabled(false);
        ui->tab->setDisabled(false);
        ui->InsertButton->setDisabled(false);
        ui->workersButton->setDisabled(false);
        if (bux) {
            bux->show();
            emit buxFormUp();
        }
        break;
    case 1: ui->makersButton->setDisabled(true);
        ui->custoersButton->setDisabled(false);
        ui->billButton->setDisabled(false);
        ui->tab->setDisabled(false);
        ui->InsertButton->setDisabled(true);
        ui->workersButton->setDisabled(true);
        manform->show();
        kass->hide();
        bux->hide();
        emit managerFormUp();
        break;
    case 2: ui->makersButton->setDisabled(true);
        ui->custoersButton->setDisabled(true);
        ui->billButton->setDisabled(false);
        ui->tab->setDisabled(true);
        ui->InsertButton->setDisabled(true);
        ui->workersButton->setDisabled(true);
        on_billButton_clicked();
        kass->show();
        manform->hide();
        bux->hide();
        break;
    case 3: ui->custoersButton->setDisabled(true);
        ui->makersButton->setDisabled(false);
        ui->billButton->setDisabled(true);
        ui->tab->setDisabled(false);
        ui->InsertButton->setDisabled(false);
        ui->workersButton->setDisabled(true);
        kass->hide();
        manform->hide();
        bux->hide();
        break;
    }

}

void MainWindow::on_tabBill_cellChanged(int row, int column)
{
    qDebug() << "MainWindow::on_tabBill_cellChanged";
    if (row != cellEnteredBillRow) return;
    cellEnteredBillRow = -1;
    QTableWidgetItem *item;
    int id = ui->tabBill->item(row,0)->text().toInt();
    qDebug() << "Bill id = " << id << " row " << row << " col " << column;
    item = ui->tabBill->item(row,column);
    QString s;
    s.sprintf("%i",id);
    switch (column)
    {
    case 0: return;
    case 1: {
        qDebug() << row << " x " << column;
        QComboBox* cb(qobject_cast<QComboBox*>(ui->tabBill->cellWidget(row, column)));
        QSqlQuery q;//(db);
        qDebug() << cb->currentText();
        res = q.exec("select id from clients where name = \'"+cb->currentText()+"\'");
        if (res) l->setText("bill up ok 1");
        else l->setText("bill up err 1");
        q.next();
        QString client(q.value(0).toString());
//        res = query.exec("select name from clients");
//        while (query.next())
//        {

//        }
        res = query.exec("update bill set client_id="+client+" where id="+s);
        if (res) l->setText("bill up ok");
        else l->setText("bill up err");
        if (res) qDebug("bill up ok");
        else qDebug("bill up err");
        break;
        //    res = query.exec("update goods set name=\'"+item->text()+"\' where id="+s); break;
    }
    case 2: {
        qDebug() << row << " x " << column;
        QComboBox* cb(qobject_cast<QComboBox*>(ui->tabBill->cellWidget(row, column)));
        QSqlQuery q;//(db);
        qDebug() << cb->currentText() << "hhkjhkj";
        res = q.exec("select id from workers where name = \'"+cb->currentText()+"\'");
        qDebug() << cb->currentText() << "hhkjhkj";
        if (res) l->setText("bill up ok 1");
        else l->setText("bill up err 1");
        qDebug() << cb->currentText() << "hhkjhkj";
        q.next();
        QString client(q.value(0).toString());
//        res = query.exec("select name from clients");
//        while (query.next())
//        {

//        }
        qDebug() << client;
        res = query.exec("update bill set manager_id="+client+" where id="+s);
        if (res) l->setText("bill up ok");
        else l->setText("bill up err");
        ///res = query.exec("update bill set supplier_id="+item->text()+" where id="+s);
        break;
        /// ---->>>>>>>>>>>>>>    не работает !!!!!!!!!!!!!!!!!!!
    }
    case 3: res = query.exec("update bill set date="+item->text()+" where id="+s); break;
    case 4: res = query.exec("update bill set payed="+item->text()+" where id="+s); break;
    default: return;
    }
    //on_showBillButton_clicked();
}

void MainWindow::on_showCustButton_clicked()
{
    ui->tableCust->setRowCount(0);
    res = query.exec("select * from clients");
    if (res)  l->setText("select ok");
    else l->setText("select error");
    int row = 0;
    QTableWidgetItem *item;
    while (query.next()) {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        qDebug() << id << ' ' << name;
        ui->tableCust->insertRow(row);
        (item = new QTableWidgetItem(id))->setFlags(Qt::NoItemFlags);
        ui->tableCust->setItem(row,0,item);
        ui->tableCust->setItem(row,1,new QTableWidgetItem(name));
        ui->tableCust->setItem(row,2,new QTableWidgetItem( query.value(2).toString() ) );
        ui->tableCust->setItem(row,3,new QTableWidgetItem( query.value(3).toString() ) );
        ui->tableCust->setItem(row,4,new QTableWidgetItem( query.value(4).toString() ) );
        ++row;
    }
}

void MainWindow::on_addCustButton_clicked()
{
    res = query.exec("select id from clients order by id desc limit 1");
    if (res) l->setText("insert select ok");
    else l->setText("insert select error");
    query.next();
    int newid = query.value(0).toInt() + 1;
    qDebug() << newid;
    QString s;
    s.sprintf("%i",newid);
    qDebug() << s;
    res = query.exec("insert into clients (id, name, addr, phone, type) values ("+s+", 'name','addr','+7(900)',1)");
    if (res) qDebug("insert ok");
    else qDebug("insert fail");
    on_showCustButton_clicked();
}

void MainWindow::on_tableCust_cellClicked(int row, int /*column*/)
{
    cellEnteredCustRow = row;
    qDebug() << "tabCustRow = "<< row;
}

void MainWindow::on_tableCust_cellChanged(int row, int column)
{
    if (row != cellEnteredCustRow) return;
    cellEnteredCustRow = -1;
    QTableWidgetItem *item = ui->tableCust->item(row,0);
    int id = item->text().toInt();
    qDebug() << "id = " << id;
    item = ui->tableCust->item(row,column);
    QString s;
    s.sprintf("%i",id);
    switch (column)
    {
    case 0: return;
    case 1: res = query.exec("update clients set name=\'"+item->text()+"\' where id="+s); break;
    case 2: res = query.exec("update clients set addr=\'"+item->text()+"\' where id="+s); break;
    case 3: res = query.exec("update clients set phone=\'"+item->text()+"\' where id="+s); break;
    case 4: res = query.exec("update clients set type=\'"+item->text()+"\' where id="+s); break;
    default: return;
    }
}

void MainWindow::on_userButton_clicked()
{
    users->show();
}

void MainWindow::slot_users(QString u,QString p)
{
    qDebug() << u << ' ' << p;
    query.exec("select user,pass,type from workers");
    QByteArray hash = QCryptographicHash::hash(p.toUtf8(), QCryptographicHash::Md5);
    qDebug() << hash.toHex();;
    while (query.next())
    {
        if (u == query.value(0).toString()) {
            if (hash.toHex() == query.value(1).toString()) {
                ui->comboBoxUser->setCurrentIndex(query.value(2).toInt());
                l->setText("Привет, "+u+"! Ты являешься "+ui->comboBoxUser->currentText());
            }
            else l->setText("Неверный пароль!");
            return;
        }
    }
    l->setText("Пользователь не найден!");
}

void MainWindow::on_workersButton_clicked()
{
    ui->bills->hide();
    ui->customers->hide();
    ui->makers->hide();
    ui->workers->show();
}

void MainWindow::on_showWorkersButton_clicked()
{
    ui->tableWorkers->setRowCount(0);
    res = query.exec("select * from workers");
    if (res)  l->setText("select ok");
    else l->setText("select error");
    int row = 0;
    QTableWidgetItem *item;
    while (query.next()) {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        qDebug() << id << ' ' << name;
        ui->tableWorkers->insertRow(row);
        (item = new QTableWidgetItem(id))->setFlags(Qt::NoItemFlags);
        ui->tableWorkers->setItem(row,0,item);
        ui->tableWorkers->setItem(row,1,new QTableWidgetItem(name));
        ui->tableWorkers->setItem(row,2,new QTableWidgetItem( query.value(2).toString() ) );
        ui->tableWorkers->setItem(row,3,new QTableWidgetItem( query.value(3).toString() ) );
        ui->tableWorkers->setItem(row,4,new QTableWidgetItem( query.value(4).toString() ) );
        if (query.value(5).toString().length() > 0)
            ui->tableWorkers->setItem(row,5,new QTableWidgetItem( "задан" ));
        else ui->tableWorkers->setItem(row,5,new QTableWidgetItem( "не задан" ));
        ++row;
    }
}
