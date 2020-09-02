#include "queryresult.h"
#include "ui_queryresult.h"

#include <QTextDocument>

#include <QDebug>

QueryResult::QueryResult(QMap<QString, Resident> & database, QString & ID, QString & phone, QString & age, QString & name, int & gender, QString & prNumber, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryResult)
{
    ui->setupUi(this);
    connect(ui->pBtn_previousRecord, SIGNAL(clicked()), this, SLOT(OnPreviousRecord()));
    connect(ui->pBtn_rearRecord, SIGNAL(clicked()), this, SLOT(OnRearRecord()));
    connect(ui->pBtn_submit, SIGNAL(clicked()), this, SLOT(OnSubmit()));
    connect(ui->pBtn_reset, SIGNAL(clicked()), this, SLOT(OnReset()));
    connect(ui->pBtn_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));


    this->database = &database;
    residentArray = database.values().toVector();
    int population = residentArray.size();
    QVector<int> availableFlag(population);
    std::fill(availableFlag.begin(), availableFlag.end(), 1);

    // gender
    if(gender != 0){
        for(int i = 0; i < population; ++i){
            if(residentArray[i].gender != gender){
                availableFlag[i] = 0;
            }
        }
    }

    // phone
    if(phone != ""){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if(!stringCompare(phone, residentArray[i].phone))
                    availableFlag[i] = 0;
            }
        }
    }

    // ID
    if(ID != ""){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if(!stringCompare(ID, residentArray[i].ID))
                    availableFlag[i] = 0;
            }
        }
    }

    // age
    if(age != ""){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                QString ageID = QString::number(ID2age(residentArray[i].ID));
                if(!stringCompare(age, ageID))
                    availableFlag[i] = 0;
            }
        }
    }

    // name
    if(name != ""){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if(!stringCompare(name, residentArray[i].name))
                    availableFlag[i] = 0;
            }
        }
    }

    // prNumber
    if(prNumber != ""){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                QStringList strList1 = prNumber.split("-");
                QStringList strList2 = residentArray[i].prNumber.split("-");
                if(!(stringCompare(strList1[0], strList2[0]) && stringCompare(strList1[1], strList2[1]) && stringCompare(strList1[2], strList2[2])))
                    availableFlag[i] = 0;
            }
        }
    }

    // store all ID of result into a qvector
    for(int i = 0; i < population; ++i){
        if(availableFlag[i] == 1){
            resultID.push_back(residentArray[i].ID);
        }
    }

    if(resultID.empty()){
        ui->label_resultRecord->setText("查询结果：没有记录");
    }else{
        display();
    }

    qDebug() << "In constructor of QueryResult, the size of result is " << resultID.size();
}

QueryResult::~QueryResult()
{
    delete ui;
}

/**
 * @brief QueryResult::stringCompare
 * @param str1
 * @param str2
 * @return Return true if str1 equals to str2.
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
    ui->label_resultRecord->setText("查询结果：" + QString::number(recordIndex + 1) + "/" + QString::number(resultID.size()));
    ui->lineEdit_ID->setText((*database)[resultID[recordIndex]].ID);
    ui->lineEdit_name->setText((*database)[resultID[recordIndex]].name);
    ui->cbBox_gender->setCurrentIndex((*database)[resultID[recordIndex]].gender);
    ui->lineEdit_prNumber->setText((*database)[resultID[recordIndex]].prNumber);
    ui->cbBox_familyStatus->setCurrentIndex((*database)[resultID[recordIndex]].familyStatus);
    ui->lineEdit_residentialAddress->setText((*database)[resultID[recordIndex]].residentialAddress);
    ui->lineEdit_domicile->setText((*database)[resultID[recordIndex]].domicile);
    ui->lineEdit_phone->setText((*database)[resultID[recordIndex]].phone);
    ui->lineEdit_nation->setText((*database)[resultID[recordIndex]].nation);
    QString birthday = (*database)[resultID[recordIndex]].ID.mid(6, 8);
    ui->label_birthday->setText(birthday.mid(0, 4) + "-" + birthday.mid(4, 2) + "-" + birthday.mid(6, 2));
    ui->cbBox_educationLevel->setCurrentIndex((*database)[resultID[recordIndex]].educationLevel);
    ui->lineEdit_occupation->setText((*database)[resultID[recordIndex]].occupation);
    ui->cbBox_militaryService->setCurrentIndex((*database)[resultID[recordIndex]].militaryService);
    ui->cbBox_maritalStatus->setCurrentIndex((*database)[resultID[recordIndex]].maritalStatus);
    ui->lineEdit_maritalDate->setText((*database)[resultID[recordIndex]].maritalDate);
    ui->lineEdit_marriageNumber->setText((*database)[resultID[recordIndex]].marriageNumber);
    ui->lineEdit_spouseInfo->setText((*database)[resultID[recordIndex]].spouseInfo);
    ui->lineEdit_childrenAmount->setText((*database)[resultID[recordIndex]].childrenAmount);
    ui->lineEdit_childrenName->setText((*database)[resultID[recordIndex]].childrenName);
    ui->cbBox_contraception->setCurrentIndex((*database)[resultID[recordIndex]].contraception);
    ui->cbBox_onlyChild->setCurrentIndex((*database)[resultID[recordIndex]].onlyChild);
    ui->cbBox_entitledGroup->setCurrentIndex((*database)[resultID[recordIndex]].entitledGroup);
    ui->cbBox_entitledCategory->setCurrentIndex((*database)[resultID[recordIndex]].entitledCategory);
    ui->cbBox_partyMember->setCurrentIndex((*database)[resultID[recordIndex]].partyMember);
    ui->cbBox_subsistenceAllowance->setCurrentIndex((*database)[resultID[recordIndex]].subsistenceAllowance);
    ui->lineEdit_allowanceAmount->setText((*database)[resultID[recordIndex]].allowanceAmount);
    ui->cbBox_retirement->setCurrentIndex((*database)[resultID[recordIndex]].retirement);
    ui->lineEdit_pension->setText((*database)[resultID[recordIndex]].pension);
    ui->cbBox_healthInsurance->setCurrentIndex((*database)[resultID[recordIndex]].healthInsurance);
    ui->lineEdit_immigrationDate->setText((*database)[resultID[recordIndex]].immigrationDate);
    ui->lineEdit_immigrationDistrict->setText((*database)[resultID[recordIndex]].immigrationDistrict);
    ui->lineEdit_emigrationDate->setText((*database)[resultID[recordIndex]].emigrationDate);
    ui->lineEdit_emigrationCause->setText((*database)[resultID[recordIndex]].emigrationCause);
    ui->pTextEdit_notes->setPlainText((*database)[resultID[recordIndex]].notes);
}

void QueryResult::OnPreviousRecord(){
    if(recordIndex == 0
            ||resultID.size() == 0){
        return;
    }else{
        --recordIndex;
        display();
    }
}

void QueryResult::OnRearRecord(){
    if(recordIndex == resultID.size() - 1
            || resultID.size() == 0){
        return;
    }else{
        ++recordIndex;
        display();
    }
}

void QueryResult::OnSubmit(){
    (*database)[resultID[recordIndex]].ID = ui->lineEdit_ID->text();
    (*database)[resultID[recordIndex]].name = ui->lineEdit_name->text();
    (*database)[resultID[recordIndex]].gender = ui->cbBox_gender->currentIndex();
    (*database)[resultID[recordIndex]].prNumber = ui->lineEdit_prNumber->text();
    (*database)[resultID[recordIndex]].familyStatus = ui->cbBox_familyStatus->currentIndex();
    (*database)[resultID[recordIndex]].residentialAddress = ui->lineEdit_residentialAddress->text();
    (*database)[resultID[recordIndex]].domicile = ui->lineEdit_domicile->text();
    (*database)[resultID[recordIndex]].phone = ui->lineEdit_phone->text();
    (*database)[resultID[recordIndex]].nation = ui->lineEdit_nation->text();
    (*database)[resultID[recordIndex]].educationLevel = ui->cbBox_educationLevel->currentIndex();
    (*database)[resultID[recordIndex]].occupation = ui->lineEdit_occupation->text();
    (*database)[resultID[recordIndex]].militaryService = ui->cbBox_militaryService->currentIndex();
    (*database)[resultID[recordIndex]].maritalStatus = ui->cbBox_maritalStatus->currentIndex();
    (*database)[resultID[recordIndex]].maritalDate = ui->lineEdit_maritalDate->text();
    (*database)[resultID[recordIndex]].marriageNumber = ui->lineEdit_marriageNumber->text();
    (*database)[resultID[recordIndex]].spouseInfo = ui->lineEdit_spouseInfo->text();
    (*database)[resultID[recordIndex]].childrenAmount = ui->lineEdit_childrenAmount->text();
    (*database)[resultID[recordIndex]].childrenName = ui->lineEdit_childrenName->text();
    (*database)[resultID[recordIndex]].contraception = ui->cbBox_contraception->currentIndex();
    (*database)[resultID[recordIndex]].onlyChild = ui->cbBox_onlyChild->currentIndex();
    (*database)[resultID[recordIndex]].entitledGroup = ui->cbBox_entitledGroup->currentIndex();
    (*database)[resultID[recordIndex]].entitledCategory = ui->cbBox_entitledCategory->currentIndex();
    (*database)[resultID[recordIndex]].partyMember = ui->cbBox_partyMember->currentIndex();
    (*database)[resultID[recordIndex]].subsistenceAllowance = ui->cbBox_subsistenceAllowance->currentIndex();
    (*database)[resultID[recordIndex]].allowanceAmount = ui->lineEdit_allowanceAmount->text();
    (*database)[resultID[recordIndex]].retirement = ui->cbBox_retirement->currentIndex();
    (*database)[resultID[recordIndex]].pension = ui->lineEdit_pension->text();
    (*database)[resultID[recordIndex]].healthInsurance = ui->cbBox_healthInsurance->currentIndex();
    (*database)[resultID[recordIndex]].immigrationDate = ui->lineEdit_immigrationDate->text();
    (*database)[resultID[recordIndex]].immigrationDistrict = ui->lineEdit_immigrationDistrict->text();
    (*database)[resultID[recordIndex]].emigrationDate = ui->lineEdit_emigrationDate->text();
    (*database)[resultID[recordIndex]].emigrationCause = ui->lineEdit_emigrationCause->text();
    (*database)[resultID[recordIndex]].notes = ui->pTextEdit_notes->toPlainText();

    accept();
}

void QueryResult::OnReset(){
    if(resultID.size() == 0){
        return;
    }else{
        display();
    }
}

void QueryResult::OnCancel(){
    reject();
}




