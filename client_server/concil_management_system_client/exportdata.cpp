#include "exportdata.h"
#include "ui_exportdata.h"
#include "saveexcel.h"
#include "displaydata.h"

#include <QDate>
#include <QDebug>

ExportData::ExportData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportData)
{
    ui->setupUi(this);
    connect(ui->pBtn_enquiry, SIGNAL(clicked()), this, SLOT(OnEnquiry()));
    connect(ui->pBtn_return, SIGNAL(clicked()), this, SLOT(OnReturn()));
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
    int groupNumber = ui->cbBox_groupNumber->currentIndex();
    int age = ui->lineEdit_age->text().toInt();
    int onlyChild = ui->cbBox_onlyChild->currentIndex();
    int childrenAmount = ui->lineEdit_childrenAmount->text().toInt();
    int maritalStatus = ui->cbBox_maritalStatus->currentIndex();
    int entitledGroup = ui->cbBox_entitledGroup->currentIndex();
    int subsistenceAllowance = ui->cbBox_subsistenceAllowance->currentIndex();
    int immigration = ui->cbBox_immigration->currentIndex();
    int emigration = ui->cbBox_emigration->currentIndex();
    int occupation = ui->cbBox_occupation->currentIndex();
    int retirement = ui->cbBox_retirement->currentIndex();
    int partyMember = ui->cbBox_partyMember->currentIndex();

    QString keywords = QString("7 0 %1%%2%%3%%4%%5%%6%%7%%8%%9%%10%%11%%12").arg(groupNumber).arg(age).arg(onlyChild).arg(childrenAmount)
                        .arg(maritalStatus).arg(entitledGroup).arg(subsistenceAllowance).arg(immigration).arg(emigration).arg(occupation)
                        .arg(retirement).arg(partyMember);
    qDebug() << "keywords in function OnEnquiry of Class ExportData : " << keywords;

    emit send2Server(keywords);
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

void ExportData::ReceiveServer(QString& str){
    if(str[2] == '-'){
        QMessageBox::information(this, tr("信息"), tr("没有找到数据"));
        return;
    }else{
        searchResult.clear();

        // json to Class Resident
        QJsonDocument jd = QJsonDocument::fromJson(str.toUtf8());
        int population = jd.array().size();

        for (int row = 0; row < population; ++row){
            ID = jd[row].toObject().value("ID").toString();
            name = jd[row].toObject().value("name").toString();
            gender = jd[row].toObject().value("gender").toInt();
            prNumber = jd[row].toObject().value("prNumber").toString();
            familyStatus = jd[row].toObject().value("familyStatus").toInt();
            residentialAddress = jd[row].toObject().value("residentialAddress").toString();
            domicile = jd[row].toObject().value("domicile").toString();
            phone = jd[row].toObject().value("phone").toString();
            nation = jd[row].toObject().value("nation").toString();
            educationLevel = jd[row].toObject().value("educationLevel").toInt();
            occupation = jd[row].toObject().value("occupation").toString();
            militaryService = jd[row].toObject().value("militaryService").toInt();
            maritalStatus = jd[row].toObject().value("maritalStatus").toInt();
            maritalDate = jd[row].toObject().value("maritalDate").toString();
            marriageNumber = jd[row].toObject().value("marriageNumber").toString();
            spouseInfo = jd[row].toObject().value("spouseInfo").toString();
            childrenAmount = jd[row].toObject().value("childrenAmount").toString();
            childrenName = jd[row].toObject().value("childrenName").toString();
            contraception = jd[row].toObject().value("contraception").toInt();
            onlyChild = jd[row].toObject().value("onlyChild").toInt();
            entitledGroup = jd[row].toObject().value("entitledGroup").toInt();
            entitledCategory = jd[row].toObject().value("entitledCategory").toInt();
            partyMember = jd[row].toObject().value("partyMember").toInt();
            subsistenceAllowance = jd[row].toObject().value("subsistenceAllowance").toInt();
            allowanceAmount = jd[row].toObject().value("allowanceAmount").toString();
            retirement = jd[row].toObject().value("retirement").toInt();
            pension = jd[row].toObject().value("pension").toString();
            healthInsurance = jd[row].toObject().value("healthInsurance").toInt();
            immigrationDate = jd[row].toObject().value("immigrationDate").toString();
            immigrationDistrict = jd[row].toObject().value("immigrationDistrict").toString();
            emigrationDate = jd[row].toObject().value("emigrationDate").toString();
            emigrationCause = jd[row].toObject().value("emigrationCause").toString();
            notes = jd[row].toObject().value("notes").toString();

            searchResult.push_back(Resident(ID, name, gender, prNumber, familyStatus, residentialAddress, domicile, phone, nation, educationLevel,
                                   occupation, militaryService, maritalStatus, maritalDate, marriageNumber, spouseInfo, childrenAmount,
                                   childrenName, contraception, onlyChild, entitledGroup, entitledCategory, partyMember, subsistenceAllowance,
                                   allowanceAmount, retirement, pension, healthInsurance, immigrationDate, immigrationDistrict, emigrationDate,
                                   emigrationCause, notes));
        }
        DisplayData *dd;
        dd = new DisplayData(searchResult, this);
        dd->setWindowFlag(Qt::Window);
        dd->show();
    }


}
