#include "adddata.h"
#include "ui_adddata.h"

AddData::AddData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddData)
{
    ui->setupUi(this);

    connect(ui->pBtn_submit, SIGNAL(clicked()), this, SLOT(OnSubmit()));
    connect(ui->pBtn_reset, SIGNAL(clicked()), this, SLOT(OnReset()));
    connect(ui->pBtn_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
    connect(ui->pBtn_birthday, SIGNAL(clicked()), this, SLOT(OnBirthdayUpdate()));
}

AddData::~AddData()
{
    delete ui;
}

void AddData::OnSubmit(){
    QJsonObject jo;
    jo.insert("ID", ui->lineEdit_ID->text().trimmed());
    jo.insert("name", ui->lineEdit_name->text().trimmed());
    jo.insert("gender", ui->cbBox_gender->currentIndex());
    jo.insert("prNumber", ui->lineEdit_prNumber->text().trimmed());
    jo.insert("familyStatus", ui->cbBox_familyStatus->currentIndex());
    jo.insert("residentialAddress", ui->lineEdit_residentialAddress->text().trimmed());
    jo.insert("domicile", ui->lineEdit_domicile->text().trimmed());
    jo.insert("phone", ui->lineEdit_phone->text().trimmed());
    jo.insert("nation", ui->lineEdit_nation->text().trimmed());
    jo.insert("educationLevel", ui->cbBox_educationLevel->currentIndex());
    jo.insert("occupation", ui->lineEdit_occupation->text().trimmed());
    jo.insert("militaryService", ui->cbBox_militaryService->currentIndex());
    jo.insert("maritalStatus", ui->cbBox_maritalStatus->currentIndex());
    jo.insert("maritalDate", ui->lineEdit_maritalDate->text().trimmed());
    jo.insert("marriageNumber", ui->lineEdit_marriageNumber->text().trimmed());
    jo.insert("spouseInfo", ui->lineEdit_spouseInfo->text().trimmed());
    jo.insert("childrenAmount", ui->lineEdit_childrenAmount->text().trimmed());
    jo.insert("childrenName", ui->lineEdit_childrenName->text().trimmed());
    jo.insert("contraception", ui->cbBox_contraception->currentIndex());
    jo.insert("onlyChild", ui->cbBox_onlyChild->currentIndex());
    jo.insert("entitledGroup", ui->cbBox_entitledGroup->currentIndex());
    jo.insert("entitledCategory", ui->cbBox_entitledCategory->currentIndex());
    jo.insert("partyMember", ui->cbBox_partyMember->currentIndex());
    jo.insert("subsistenceAllowance", ui->cbBox_subsistenceAllowance->currentIndex());
    jo.insert("allowanceAmount", ui->lineEdit_allowanceAmount->text().trimmed());
    jo.insert("retirement", ui->cbBox_retirement->currentIndex());
    jo.insert("pension", ui->lineEdit_pension->text().trimmed());
    jo.insert("healthInsurance", ui->cbBox_healthInsurance->currentIndex());
    jo.insert("immigrationDate", ui->lineEdit_immigrationDate->text().trimmed());
    jo.insert("immigrationDistrict", ui->lineEdit_immigrationDistrict->text().trimmed());
    jo.insert("emigrationDate", ui->lineEdit_emigrationDate->text().trimmed());
    jo.insert("emigrationCause", ui->lineEdit_emigrationCause->text().trimmed());
    jo.insert("notes", ui->pTextEdit_notes->toPlainText().trimmed());

    QJsonArray ja;
    ja.append(jo);
    QJsonDocument jd;
    jd.setArray(ja);
    QString json = jd.toJson();
    emit send2Server(json);

    accept();
}

void AddData::OnReset(){
    ui->lineEdit_ID->clear();
    ui->lineEdit_name->clear();
    ui->cbBox_gender->setCurrentIndex(0);
    ui->lineEdit_prNumber->clear();
    ui->cbBox_familyStatus->setCurrentIndex(0);
    ui->lineEdit_residentialAddress->clear();
    ui->lineEdit_domicile->clear();
    ui->lineEdit_phone->clear();
    ui->lineEdit_nation->setText(QString("汉"));
//    ui->lineEdit_birthday->setText(QString("1800-01-01"));
    ui->label_birthday->setText(QString("1800-01-01"));
    ui->cbBox_educationLevel->setCurrentIndex(0);
    ui->lineEdit_occupation->clear();
    ui->cbBox_militaryService->setCurrentIndex(0);
    ui->cbBox_maritalStatus->setCurrentIndex(0);
    ui->lineEdit_maritalDate->setText(QString("1800-01-01"));
    ui->lineEdit_marriageNumber->clear();
    ui->lineEdit_spouseInfo->clear();
    ui->lineEdit_childrenAmount->clear();
    ui->lineEdit_childrenName->clear();
    ui->cbBox_contraception->setCurrentIndex(0);
    ui->cbBox_onlyChild->setCurrentIndex(0);
    ui->cbBox_entitledGroup->setCurrentIndex(0);
    ui->cbBox_entitledCategory->setCurrentIndex(0);
    ui->cbBox_partyMember->setCurrentIndex(0);
    ui->cbBox_subsistenceAllowance->setCurrentIndex(0);
    ui->lineEdit_allowanceAmount->clear();
    ui->cbBox_retirement->setCurrentIndex(0);
    ui->lineEdit_pension->clear();
    ui->cbBox_healthInsurance->setCurrentIndex(0);
    ui->lineEdit_immigrationDate->setText(QString("1800-01-01"));
    ui->lineEdit_immigrationDistrict->clear();
    ui->lineEdit_emigrationDate->setText(QString("1800-01-01"));
    ui->lineEdit_emigrationCause->clear();
    ui->pTextEdit_notes->clear();
}

void AddData::OnCancel(){
    reject();
}

void AddData::OnBirthdayUpdate(){
    QString year, month, day;
    QString birthday = ui->lineEdit_ID->text();
    if(birthday.length() != 18){
        return;
    } else {
        birthday = birthday.mid(6, 8);
    }

    if(birthday.toULong()){
        year = birthday.mid(0, 4);
        month = birthday.mid(4, 2);
        day = birthday.mid(6);
    } else {
        return;
    }

    ui->label_birthday->setText(year + "-" + month + "-" + day);
}

void AddData::ReceiveServer(QString& result){
    if(result[2].unicode() == '0'){
        QMessageBox::information(this, tr("通知"), tr("添加数据失败"));
    }else if(result[2].unicode() == '1'){
        QMessageBox::information(this, tr("通知"), tr("添加数据成功"));
    }else{
        QMessageBox::information(this, tr("通知"), tr("通信异常"));
    }
}
