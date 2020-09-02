#include "querydata.h"
#include "ui_querydata.h"

QueryData::QueryData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryData)
{
    ui->setupUi(this);
    connect(ui->pBtn_enquiry, SIGNAL(clicked()), this, SLOT(OnEnquiry()));
    connect(ui->pBtn_return, SIGNAL(clicked()), this, SLOT(OnReturn()));
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

    QString keywords = QString("6 0 %1%%2%%3%%4%%5%%6").arg(ID).arg(phone).arg(age).arg(name).arg(gender).arg(prNumber);
    emit send2Server(keywords);

    qDebug() << "keywords in function OnEnquiry of Class ExportData : " << keywords;
}

void QueryData::OnReturn(){
    reject();
}

/**
 * @brief QueryData::ReceiveServer
 * @param str
 */
void QueryData::ReceiveServer(QString& str){
    if(str[0] == '6'){
        if(str[2] == '-'){
            QMessageBox::information(this, QString("信息"), QString("未找到任何数据"));
        }else if(str[2] == 't' || str[2] == 'f'){
            emit send2AlterResult(str);
        }else{
            QMessageBox::information(this, QString("信息"), QString("通信异常"));
        }
    }else{
        residentList.clear();

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

            residentList.push_back(Resident(ID, name, gender, prNumber, familyStatus, residentialAddress, domicile, phone, nation, educationLevel,
                                   occupation, militaryService, maritalStatus, maritalDate, marriageNumber, spouseInfo, childrenAmount,
                                   childrenName, contraception, onlyChild, entitledGroup, entitledCategory, partyMember, subsistenceAllowance,
                                   allowanceAmount, retirement, pension, healthInsurance, immigrationDate, immigrationDistrict, emigrationDate,
                                   emigrationCause, notes));
        }

        qr = new QueryResult(residentList);
        connect(qr, SIGNAL(replyResident(Resident&)), this, SLOT(ReceiveAlteredResident(Resident&)));
        connect(this, SIGNAL(send2AlterResult(QString&)), qr, SLOT(AlteredResult(QString&)));
        qr->exec();
    }
}

/**
 * @brief QueryData::ReceiveAlteredResident : receive modified resident instance from QueryResult, transform it into json format and send to server
 * @param rd
 */
void QueryData::ReceiveAlteredResident(Resident& rd){
    QJsonArray ja;
    ja.append(Resident2QJsonObject(rd));
    QJsonDocument jd;
    jd.setArray(ja);
    QString jsonResident = jd.toJson();
    QString mes = QString("6 * %1").arg(jsonResident);
    emit send2Server(mes);
}

QJsonObject QueryData::Resident2QJsonObject(const Resident& res){
    QJsonObject jo;
    jo.insert("ID", res.ID);
    jo.insert("name", res.name);
    jo.insert("gender", res.gender);
    jo.insert("prNumber", res.prNumber);
    jo.insert("familyStatus", res.familyStatus);
    jo.insert("residentialAddress", res.residentialAddress);
    jo.insert("domicile", res.domicile);
    jo.insert("phone", res.phone);
    jo.insert("nation", res.nation);
    jo.insert("educationLevel", res.educationLevel);
    jo.insert("occupation", res.occupation);
    jo.insert("militaryService", res.militaryService);
    jo.insert("maritalStatus", res.maritalStatus);
    jo.insert("maritalDate", res.maritalDate);
    jo.insert("marriageNumber", res.marriageNumber);
    jo.insert("spouseInfo", res.spouseInfo);
    jo.insert("childrenAmount", res.childrenAmount);
    jo.insert("childrenName", res.childrenName);
    jo.insert("contraception", res.contraception);
    jo.insert("onlyChild", res.onlyChild);
    jo.insert("entitledGroup", res.entitledGroup);
    jo.insert("entitledCategory", res.entitledCategory);
    jo.insert("partyMember", res.partyMember);
    jo.insert("subsistenceAllowance", res.subsistenceAllowance);
    jo.insert("allowanceAmount", res.allowanceAmount);
    jo.insert("retirement", res.retirement);
    jo.insert("pension", res.pension);
    jo.insert("healthInsurance", res.healthInsurance);
    jo.insert("immigrationDate", res.immigrationDate);
    jo.insert("immigrationDistrict", res.immigrationDistrict);
    jo.insert("emigrationDate", res.emigrationDate);
    jo.insert("emigrationCause", res.emigrationCause);
    jo.insert("notes", res.notes);

    return jo;
}
