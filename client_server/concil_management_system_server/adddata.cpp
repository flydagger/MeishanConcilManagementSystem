#include "adddata.h"
#include "ui_adddata.h"

AddData::AddData(const Resident &rd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddData)
{
    ui->setupUi(this);

    connect(ui->pBtn_submit, SIGNAL(clicked()), this, SLOT(OnSubmit()));
    connect(ui->pBtn_reset, SIGNAL(clicked()), this, SLOT(OnReset()));
    connect(ui->pBtn_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
    connect(ui->pBtn_birthday, SIGNAL(clicked()), this, SLOT(OnBirthdayUpdate()));
    newPersonInfo = rd;
}

AddData::~AddData()
{
    delete ui;
}

void AddData::OnSubmit(){
    newPersonInfo.ID = ui->lineEdit_ID->text();
    newPersonInfo.name = ui->lineEdit_name->text();
    newPersonInfo.gender = ui->cbBox_gender->currentIndex();
    newPersonInfo.prNumber = ui->lineEdit_prNumber->text();
    newPersonInfo.familyStatus = ui->cbBox_familyStatus->currentIndex();
    newPersonInfo.residentialAddress = ui->lineEdit_residentialAddress->text();
    newPersonInfo.domicile = ui->lineEdit_domicile->text();
    newPersonInfo.phone = ui->lineEdit_phone->text();
    newPersonInfo.nation = ui->lineEdit_nation->text();
//    newPersonInfo.birthday = ui->lineEdit_birthday->text();
    newPersonInfo.educationLevel = ui->cbBox_educationLevel->currentIndex();
    newPersonInfo.occupation = ui->lineEdit_occupation->text();
    newPersonInfo.militaryService = ui->cbBox_militaryService->currentIndex();
    newPersonInfo.maritalStatus = ui->cbBox_maritalStatus->currentIndex();
    newPersonInfo.maritalDate = ui->lineEdit_maritalDate->text();
    newPersonInfo.marriageNumber = ui->lineEdit_marriageNumber->text();
    newPersonInfo.spouseInfo = ui->lineEdit_spouseInfo->text();
    newPersonInfo.childrenAmount = ui->lineEdit_childrenAmount->text();
    newPersonInfo.childrenName = ui->lineEdit_childrenName->text();
    newPersonInfo.contraception = ui->cbBox_contraception->currentIndex();
    newPersonInfo.onlyChild = ui->cbBox_onlyChild->currentIndex();
    newPersonInfo.entitledGroup = ui->cbBox_entitledGroup->currentIndex();
    newPersonInfo.entitledCategory = ui->cbBox_entitledCategory->currentIndex();
    newPersonInfo.partyMember = ui->cbBox_partyMember->currentIndex();
    newPersonInfo.subsistenceAllowance = ui->cbBox_subsistenceAllowance->currentIndex();
    newPersonInfo.allowanceAmount = ui->lineEdit_allowanceAmount->text();
    newPersonInfo.retirement = ui->cbBox_retirement->currentIndex();
    newPersonInfo.pension = ui->lineEdit_pension->text();
    newPersonInfo.healthInsurance = ui->cbBox_healthInsurance->currentIndex();
    newPersonInfo.immigrationDate = ui->lineEdit_immigrationDate->text();
    newPersonInfo.immigrationDistrict = ui->lineEdit_immigrationDistrict->text();
    newPersonInfo.emigrationDate = ui->lineEdit_emigrationDate->text();
    newPersonInfo.emigrationCause = ui->lineEdit_emigrationCause->text();
    newPersonInfo.notes = ui->pTextEdit_notes->toPlainText();

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
