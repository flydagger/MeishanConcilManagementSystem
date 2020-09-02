#include "exportdata.h"
#include "ui_exportdata.h"
#include "saveexcel.h"
#include "displaydata.h"

#include <QDate>
#include <QDebug>

ExportData::ExportData(QMap<QString, Resident> & database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportData)
{
    ui->setupUi(this);
    connect(ui->pBtn_enquiry, SIGNAL(clicked()), this, SLOT(OnEnquiry()));
    connect(ui->pBtn_return, SIGNAL(clicked()), this, SLOT(OnReturn()));

    this->database = &database;
    residentArray = database.values().toVector();
    population = residentArray.size();
    availableFlag.resize(population);
    std::fill(availableFlag.begin(), availableFlag.end(), 1);
}

ExportData::~ExportData()
{
    delete ui;
}

int count(const QVector<int> & flagArray){
    int ret = 0;
    for(auto &i : flagArray){
        if(i == 1){
            ret++;
        }
    }
    return ret;
}

void ExportData::OnEnquiry(){
    std::fill(availableFlag.begin(), availableFlag.end(), 1);
    searchResult.clear();

    // 组号 cbBox_groupNumber(the middle part of prNumber)
    if(ui->cbBox_groupNumber->currentIndex() != 0){  // if group number is 0 which means uncertain, don't check group number.
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else if(residentArray[i].prNumber.split("-")[1].toInt() != ui->cbBox_groupNumber->currentIndex()){
                availableFlag[i] = 0;
            }
        }
    }

    // 年龄 lineEdit_age
    if(ui->lineEdit_age->text() != ""){  // if age is null, don't check age.
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else if(ID2age(residentArray[i].ID) != ui->lineEdit_age->text().toInt()){
                availableFlag[i] = 0;
            }
        }
    }

    // 独生子女 cbBox_onlyChild
    qDebug() << ui->cbBox_onlyChild->currentIndex();
    if(ui->cbBox_onlyChild->currentIndex() != 0){  // if only child is 0 which means uncertain, don't check only child.
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else if((residentArray[i].onlyChild + 1) != ui->cbBox_onlyChild->currentIndex()){
                availableFlag[i] = 0;
            }
        }
    }

    // 子女数 lineEdit_childrenAmount
    if(ui->lineEdit_childrenAmount->text() != ""){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if(residentArray[i].childrenAmount != ui->lineEdit_childrenAmount->text())
                    availableFlag[i] = 0;
            }
        }
    }

    // 婚姻状况 cbBox_maritalStatus
    if(ui->cbBox_maritalStatus->currentIndex() != 0){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if((residentArray[i].maritalStatus + 1) != ui->cbBox_maritalStatus->currentIndex())
                    availableFlag[i] = 0;
            }
        }
    }

    // 优抚对象 cbBox_entitledGroup
    if(ui->cbBox_entitledGroup->currentIndex() != 0){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if((residentArray[i].entitledGroup + 1) != ui->cbBox_entitledGroup->currentIndex())
                    availableFlag[i] = 0;
            }
        }
    }

    // 享受低保 cbBox_subsistenceAllowance
    if(ui->cbBox_subsistenceAllowance->currentIndex() != 0){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if((residentArray[i].subsistenceAllowance + 1) != ui->cbBox_subsistenceAllowance->currentIndex())
                    availableFlag[i] = 0;
            }
        }
    }

    // 迁入人员 cbBox_immigration : 2否 3是
    // 根据迁入日期来判断是否是迁入人员
    if(!(ui->cbBox_immigration->currentIndex() == 0 || ui->cbBox_immigration->currentIndex() == 1)){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if((ui->cbBox_immigration->currentIndex() == 2 && residentArray[i].immigrationDate != "1800-01-01")
                        || (ui->cbBox_immigration->currentIndex() == 3 && residentArray[i].immigrationDate == "1800-01-01")){
                    availableFlag[i] = 0;
                }
            }
        }
    }

    // 迁出人员 cbBox_emigration : 1否 2是
    // 根据迁出日期来判断是否是迁出人员
    if(!(ui->cbBox_emigration->currentIndex() == 0 || ui->cbBox_emigration->currentIndex() == 1)){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if((ui->cbBox_emigration->currentIndex() == 2 && residentArray[i].emigrationDate != "1800-01-01")
                        || (ui->cbBox_emigration->currentIndex() == 3 && residentArray[i].emigrationDate == "1800-01-01")){
                    availableFlag[i] = 0;
                }
            }
        }
    }

    // 就业人员 cbBox_occupation
    if(!(ui->cbBox_occupation->currentIndex() == 0 || ui->cbBox_occupation->currentIndex() == 1)){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if((ui->cbBox_occupation->currentIndex() == 2 && residentArray[i].occupation != QStringLiteral(""))
                        || (ui->cbBox_occupation->currentIndex() == 3 && residentArray[i].occupation == QStringLiteral(""))){
                    availableFlag[i] = 0;
                }
            }
        }
    }

    // 退休 cbBox_retirement
    if(!(ui->cbBox_retirement->currentIndex() == 0 || ui->cbBox_retirement->currentIndex() == 1)){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if(ui->cbBox_retirement->currentIndex() != (residentArray[i].retirement + 1)){
                    availableFlag[i] = 0;
                }
            }
        }
    }

    // 党员 cbBox_partyMember
    if(!(ui->cbBox_partyMember->currentIndex() == 0 || ui->cbBox_partyMember->currentIndex() == 1)){
        for(int i = 0; i < population; ++i){
            if(availableFlag[i] == 0){
                continue;
            }else{
                if((residentArray[i].partyMember + 1) != ui->cbBox_partyMember->currentIndex())
                    availableFlag[i] = 0;
            }
        }
    }

    // generate a list containing search result
    for(int i = 0; i < population; ++i){
        if(availableFlag[i] == 0){
            continue;
        }else{
            searchResult.push_back(residentArray[i]);
        }
    }

//    qDebug() << "In OnEnquiry, the size of searchResult is " << searchResult.size();
    DisplayData *dd;
    dd = new DisplayData(searchResult, this);
    dd->setWindowFlag(Qt::Window);
    dd->show();
}

void ExportData::OnReturn(){
    reject();
}

int ExportData::ID2age(const QString & ID){
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

/* Function: Compare two qstring.
 * Param: strStar - string may include *
 * Param: str - string without *
 * Retrun: true if two strings are equivalent
 */
bool ExportData::stringCompare(QString & strStar, QString & str){
    if(strStar.length() != str.length())
        return false;
    for(int i = 0; i < strStar.length(); i++){
        if(strStar[i] == "*")
            continue;
        if(strStar[i] != str[i])
            return false;
    }
    return true;
}
