#include "querydata.h"
#include "ui_querydata.h"

QueryData::QueryData(QMap<QString, Resident> & database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryData)
{
    ui->setupUi(this);
    connect(ui->pBtn_enquiry, SIGNAL(clicked()), this, SLOT(OnEnquiry()));
    connect(ui->pBtn_return, SIGNAL(clicked()), this, SLOT(OnReturn()));

    this->database = &database;
}

QueryData::~QueryData()
{
    delete ui;
}

void QueryData::OnEnquiry(){
    QString ID = ui->lineEdit_ID->text();
    QString phone = ui->lineEdit_phone->text();
    QString age = ui->lineEdit_age->text();
    QString name = ui->lineEdit_name->text();
    int gender = ui->cbBox_gender->currentIndex();
    QString prNumber = ui->lineEdit_prNumber->text();
    QueryResult qr(*this->database, ID, phone, age, name, gender, prNumber, this);
    qr.exec();
}

void QueryData::OnReturn(){
    reject();
}
