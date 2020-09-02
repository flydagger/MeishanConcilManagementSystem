#include "queryresult.h"
#include "ui_queryresult.h"

#include <QTextDocument>

#include <QDebug>

QueryResult::QueryResult(QList<Resident>& rdList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryResult)
{
    ui->setupUi(this);
    connect(ui->pBtn_previousRecord, SIGNAL(clicked()), this, SLOT(OnPreviousRecord()));
    connect(ui->pBtn_rearRecord, SIGNAL(clicked()), this, SLOT(OnRearRecord()));
    connect(ui->pBtn_submit, SIGNAL(clicked()), this, SLOT(OnSubmit()));
    connect(ui->pBtn_reset, SIGNAL(clicked()), this, SLOT(OnReset()));
    connect(ui->pBtn_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

    residentList = rdList;

    display();
    qDebug() << "In constructor of QueryResult, the size of result is " << residentList.size();
}

QueryResult::~QueryResult()
{
    delete ui;
}

/*
 *  Return true if str1 equals to str2.
 */
bool QueryResult::stringCompare(QString & str1, QString & str2){
    if(str1.length() != str2.length())
        return false;
    for(int i = 0; i < str1.length(); i++){
        if(str1[i] == "*" || str2[i] == "*")
            continue;
        if(str1[i] != str2[i])
            return false;
    }
    return true;
}

int QueryResult::ID2age(const QString & ID){
    int year = ID.mid(6, 4).toInt();
    int month = ID.mid(10, 2).toInt();
    int day = ID.mid(12, 2).toInt();

    int tYear = QDate::currentDate().year();
    int tMonth = QDate::currentDate().month();
    int tDay = QDate::currentDate().day();

    if(tMonth > month){
        return tYear - year;
    }else if(tMonth == month){
        if(tDay >= day)
            return tYear - year;
        else
            return tYear - year - 1;
    }else{
        return tYear - year - 1;
    }
}

void QueryResult::display(){
    ui->label_resultRecord->setText("查询结果：" + QString::number(recordIndex+1) + "/" + QString::number(residentList.size()));
    ui->lineEdit_ID->setText(residentList[recordIndex].ID);
    ui->lineEdit_name->setText(residentList[recordIndex].name);
    ui->cbBox_gender->setCurrentIndex(residentList[recordIndex].gender);
    ui->lineEdit_prNumber->setText(residentList[recordIndex].prNumber);
    ui->cbBox_familyStatus->setCurrentIndex(residentList[recordIndex].familyStatus);
    ui->lineEdit_residentialAddress->setText(residentList[recordIndex].residentialAddress);
    ui->lineEdit_domicile->setText(residentList[recordIndex].domicile);
    ui->lineEdit_phone->setText(residentList[recordIndex].phone);
    ui->lineEdit_nation->setText(residentList[recordIndex].nation);
    QString birthday = residentList[recordIndex].ID.mid(6, 8);
    ui->label_birthday->setText(birthday.mid(0, 4) + "-" + birthday.mid(4, 2) + "-" + birthday.mid(6, 2));
    ui->cbBox_educationLevel->setCurrentIndex(residentList[recordIndex].educationLevel);
    ui->lineEdit_occupation->setText(residentList[recordIndex].occupation);
    ui->cbBox_militaryService->setCurrentIndex(residentList[recordIndex].militaryService);
    ui->cbBox_maritalStatus->setCurrentIndex(residentList[recordIndex].maritalStatus);
    ui->lineEdit_maritalDate->setText(residentList[recordIndex].maritalDate);
    ui->lineEdit_marriageNumber->setText(residentList[recordIndex].marriageNumber);
    ui->lineEdit_spouseInfo->setText(residentList[recordIndex].spouseInfo);
    ui->lineEdit_childrenAmount->setText(residentList[recordIndex].childrenAmount);
    ui->lineEdit_childrenName->setText(residentList[recordIndex].childrenName);
    ui->cbBox_contraception->setCurrentIndex(residentList[recordIndex].contraception);
    ui->cbBox_onlyChild->setCurrentIndex(residentList[recordIndex].onlyChild);
    ui->cbBox_entitledGroup->setCurrentIndex(residentList[recordIndex].entitledGroup);
    ui->cbBox_entitledCategory->setCurrentIndex(residentList[recordIndex].entitledCategory);
    ui->cbBox_partyMember->setCurrentIndex(residentList[recordIndex].partyMember);
    ui->cbBox_subsistenceAllowance->setCurrentIndex(residentList[recordIndex].subsistenceAllowance);
    ui->lineEdit_allowanceAmount->setText(residentList[recordIndex].allowanceAmount);
    ui->cbBox_retirement->setCurrentIndex(residentList[recordIndex].retirement);
    ui->lineEdit_pension->setText(residentList[recordIndex].pension);
    ui->cbBox_healthInsurance->setCurrentIndex(residentList[recordIndex].healthInsurance);
    ui->lineEdit_immigrationDate->setText(residentList[recordIndex].immigrationDate);
    ui->lineEdit_immigrationDistrict->setText(residentList[recordIndex].immigrationDistrict);
    ui->lineEdit_emigrationDate->setText(residentList[recordIndex].emigrationDate);
    ui->lineEdit_emigrationCause->setText(residentList[recordIndex].emigrationCause);
    ui->pTextEdit_notes->setPlainText(residentList[recordIndex].notes);
}

void QueryResult::OnPreviousRecord(){
    if(recordIndex == 0 || residentList.size() == 0){
        return;
    }else{
        --recordIndex;
        display();
    }
}

void QueryResult::OnRearRecord(){
    if(recordIndex == residentList.size() - 1 || residentList.size() == 0){
        return;
    }else{
        ++recordIndex;
        display();
    }
}

void QueryResult::OnSubmit(){
    alteredResident.ID = ui->lineEdit_ID->text();
    alteredResident.name = ui->lineEdit_name->text();
    alteredResident.gender = ui->cbBox_gender->currentIndex();
    alteredResident.prNumber = ui->lineEdit_prNumber->text();
    alteredResident.familyStatus = ui->cbBox_familyStatus->currentIndex();
    alteredResident.residentialAddress = ui->lineEdit_residentialAddress->text();
    alteredResident.domicile = ui->lineEdit_domicile->text();
    alteredResident.phone = ui->lineEdit_phone->text();
    alteredResident.nation = ui->lineEdit_nation->text();
    alteredResident.educationLevel = ui->cbBox_educationLevel->currentIndex();
    alteredResident.occupation = ui->lineEdit_occupation->text();
    alteredResident.militaryService = ui->cbBox_militaryService->currentIndex();
    alteredResident.maritalStatus = ui->cbBox_maritalStatus->currentIndex();
    alteredResident.maritalDate = ui->lineEdit_maritalDate->text();
    alteredResident.marriageNumber = ui->lineEdit_marriageNumber->text();
    alteredResident.spouseInfo = ui->lineEdit_spouseInfo->text();
    alteredResident.childrenAmount = ui->lineEdit_childrenAmount->text();
    alteredResident.childrenName = ui->lineEdit_childrenName->text();
    alteredResident.contraception = ui->cbBox_contraception->currentIndex();
    alteredResident.onlyChild = ui->cbBox_onlyChild->currentIndex();
    alteredResident.entitledGroup = ui->cbBox_entitledGroup->currentIndex();
    alteredResident.entitledCategory = ui->cbBox_entitledCategory->currentIndex();
    alteredResident.partyMember = ui->cbBox_partyMember->currentIndex();
    alteredResident.subsistenceAllowance = ui->cbBox_subsistenceAllowance->currentIndex();
    alteredResident.allowanceAmount = ui->lineEdit_allowanceAmount->text();
    alteredResident.retirement = ui->cbBox_retirement->currentIndex();
    alteredResident.pension = ui->lineEdit_pension->text();
    alteredResident.healthInsurance = ui->cbBox_healthInsurance->currentIndex();
    alteredResident.immigrationDate = ui->lineEdit_immigrationDate->text();
    alteredResident.immigrationDistrict = ui->lineEdit_immigrationDistrict->text();
    alteredResident.emigrationDate = ui->lineEdit_emigrationDate->text();
    alteredResident.emigrationCause = ui->lineEdit_emigrationCause->text();
    alteredResident.notes = ui->pTextEdit_notes->toPlainText();

    emit replyResident(alteredResident);
}

void QueryResult::OnReset(){
    recordIndex = 0;
    display();
}

void QueryResult::OnCancel(){
    reject();
}

void QueryResult::AlteredResult(QString& result){
    if(result[2] == 'f'){
        ui->label_alteredResult->setText(tr("修改失败"));
    }else if(result[2] == 't'){
        ui->label_alteredResult->setText(tr("修改成功"));
    }else{
        ui->label_alteredResult->setText(tr("通信错误"));
    }
}


