#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "changepassword.h"
#include "adddata.h"
#include "querydata.h"
#include "exportdata.h"
#include "./QXlsx/xlsxdocument.h"
#include "./QXlsx/xlsxchartsheet.h"
#include "./QXlsx/xlsxcellrange.h"
#include "./QXlsx/xlsxchart.h"
#include "./QXlsx/xlsxrichstring.h"
#include "./QXlsx/xlsxworkbook.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

using namespace QXlsx;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pBtn_login, SIGNAL(clicked()), this, SLOT(OnLogin()));
    connect(ui->action_changePassword, SIGNAL(triggered()), this, SLOT(OnChangePassword()));
    connect(ui->action_importData, SIGNAL(triggered()), this, SLOT(OnImportData()));
    connect(ui->action_addData, SIGNAL(triggered()), this, SLOT(OnAddData()));
    connect(ui->action_alterData, SIGNAL(triggered()), this, SLOT(OnQueryData()));
    connect(ui->action_exportData, SIGNAL(triggered()), this, SLOT(OnExportData()));
    connect(ui->action_logout, SIGNAL(triggered()), this, SLOT(OnLogout()));
    connect(ui->action_about, SIGNAL(triggered()), this, SLOT(OnAbout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnLogin(){
    if(loginStatus){
        ui->statusBar->showMessage("不要重复登录", 5000);
        return;
    }

    QFile file("user_list.txt");
    if (!file.exists()){
        qDebug() << "File does not exist.";
    }
    if (!file.open(QIODevice::ReadOnly  | QIODevice::Text)){
        qDebug() << "Fail to open user list file.";
    }

    QString content = file.readAll();
    QStringList userList = content.split('\n');
    QStringList userKeyPair;
    QMap<QString, QString> userKeyMap;
    for(auto &i : userList){
        if(i == ""){
            continue;
        }

        userKeyPair = i.split(' ');
        userKeyMap[userKeyPair[0]] = userKeyPair[1];
    }

    QMapIterator<QString, QString> itLogin(userKeyMap);
    while(itLogin.hasNext()){
        itLogin.next();
        if(ui->lineEdit_username->text() == itLogin.key() && ui->lineEdit_password->text() == itLogin.value()){
            loginStatus = true;
            qDebug() << "Succeed to login.";
            qDebug() << "Username " << itLogin.key() << "Password " << itLogin.value();
            username = itLogin.key();
            password = itLogin.value();

            // Read database from json file
            QString val;
            QFile file;
            file.setFileName("database.json");
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            val = file.readAll();
            file.close();
            QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
            int population = d.array().size();  // 数据库中的人数

            QString ID;
            QString name;
            int gender;
            QString prNumber;
            int familyStatus;
            QString residentialAddress;
            QString domicile;
            QString phone;
            QString nation;
            int educationLevel;
            QString occupation;
            int militaryService;
            int maritalStatus;
            QString maritalDate;
            QString marriageNumber;
            QString spouseInfo;
            QString childrenAmount;
            QString childrenName;
            int contraception;
            int onlyChild;
            int entitledGroup;
            int entitledCategory;
            int partyMember;
            int subsistenceAllowance;
            QString allowanceAmount;
            int retirement;
            QString pension;
            int healthInsurance;
            QString immigrationDate;
            QString immigrationDistrict;
            QString emigrationDate;
            QString emigrationCause;
            QString notes;

            for (int row = 0; row < population; ++row){
                ID = d[row].toObject().value("ID").toString();
                name = d[row].toObject().value("name").toString();
                gender = d[row].toObject().value("gender").toInt();
                prNumber = d[row].toObject().value("prNumber").toString();
                familyStatus = d[row].toObject().value("familyStatus").toInt();
                residentialAddress = d[row].toObject().value("residentialAddress").toString();
                domicile = d[row].toObject().value("domicile").toString();
                phone = d[row].toObject().value("phone").toString();
                nation = d[row].toObject().value("nation").toString();
                educationLevel = d[row].toObject().value("educationLevel").toInt();
                occupation = d[row].toObject().value("occupation").toString();
                militaryService = d[row].toObject().value("militaryService").toInt();
                maritalStatus = d[row].toObject().value("maritalStatus").toInt();
                maritalDate = d[row].toObject().value("maritalDate").toString();
                marriageNumber = d[row].toObject().value("marriageNumber").toString();
                spouseInfo = d[row].toObject().value("spouseInfo").toString();
                childrenAmount = d[row].toObject().value("childrenAmount").toString();
                childrenName = d[row].toObject().value("childrenName").toString();
                contraception = d[row].toObject().value("contraception").toInt();
                onlyChild = d[row].toObject().value("onlyChild").toInt();
                entitledGroup = d[row].toObject().value("entitledGroup").toInt();
                entitledCategory = d[row].toObject().value("entitledCategory").toInt();
                partyMember = d[row].toObject().value("partyMember").toInt();
                subsistenceAllowance = d[row].toObject().value("subsistenceAllowance").toInt();
                allowanceAmount = d[row].toObject().value("allowanceAmount").toString();
                retirement = d[row].toObject().value("retirement").toInt();
                pension = d[row].toObject().value("pension").toString();
                healthInsurance = d[row].toObject().value("healthInsurance").toInt();
                immigrationDate = d[row].toObject().value("immigrationDate").toString();
                immigrationDistrict = d[row].toObject().value("immigrationDistrict").toString();
                emigrationDate = d[row].toObject().value("emigrationDate").toString();
                emigrationCause = d[row].toObject().value("emigrationCause").toString();
                notes = d[row].toObject().value("notes").toString();

                database[ID] = Resident(ID, name, gender, prNumber, familyStatus, residentialAddress, domicile, phone, nation, educationLevel,
                                        occupation, militaryService, maritalStatus, maritalDate, marriageNumber, spouseInfo, childrenAmount,
                                        childrenName, contraception, onlyChild, entitledGroup, entitledCategory, partyMember, subsistenceAllowance,
                                        allowanceAmount, retirement, pension, healthInsurance, immigrationDate, immigrationDistrict, emigrationDate,
                                        emigrationCause, notes);
            }

            qDebug() << "In OnLogin, the size of database is " << database.size();
            if(population > 1){
                qDebug()<< database.begin().value().ID
                        << database.begin().value().name
                        << database.begin().value().gender
                        << database.begin().value().prNumber
                        << database.begin().value().familyStatus
                        << database.begin().value().residentialAddress
                        << database.begin().value().domicile
                        << database.begin().value().phone
                        << database.begin().value().nation
                        << database.begin().value().educationLevel
                        << database.begin().value().occupation
                        << database.begin().value().militaryService
                        << database.begin().value().maritalStatus
                        << database.begin().value().maritalDate
                        << database.begin().value().marriageNumber
                        << database.begin().value().spouseInfo
                        << database.begin().value().childrenAmount
                        << database.begin().value().childrenName
                        << database.begin().value().contraception
                        << database.begin().value().onlyChild
                        << database.begin().value().entitledGroup
                        << database.begin().value().entitledCategory
                        << database.begin().value().partyMember
                        << database.begin().value().subsistenceAllowance
                        << database.begin().value().allowanceAmount
                        << database.begin().value().retirement
                        << database.begin().value().pension
                        << database.begin().value().healthInsurance
                        << database.begin().value().immigrationDate
                        << database.begin().value().immigrationDistrict
                        << database.begin().value().emigrationDate
                        << database.begin().value().emigrationCause
                        << database.begin().value().notes;
            }
            ui->statusBar->showMessage("登陆成功", 5000);
            break;
        }else{
            ui->statusBar->showMessage("登陆失败", 5000);
        }
    }
}

void MainWindow::OnChangePassword(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    qDebug() << "In Mainwindow, password is " << password;
    ChangePassword cp(password, this);
    int ret = cp.exec();
    if(ret == QDialog::Accepted){
        password = cp.originalPassword;

        // save username and password to file
        QFile userFile("user_list.txt");
        if (!userFile.exists()){
            qDebug() << "userFile does not exist.";
        }
        if (!userFile.open(QIODevice::ReadOnly  | QIODevice::Text)){
            qDebug() << "Fail to read userlist file.";
        }

        QString content = userFile.readAll();
        userFile.close();

        if (!userFile.open(QIODevice::WriteOnly  | QIODevice::Text | QIODevice::Truncate)){
            qDebug() << "Fail to truncate userlist file.";
        }

        QStringList userList = content.split('\n');
        QStringList userKeyPair;

        for(int i = 0; i < userList.size(); ++i){
            if(userList[i].split(' ')[0] == username){
                userList.replace(i, username + " " + password);
            }
        }

        for(auto &i : userList){
            qDebug() << i;
            userFile.write((i + "\n").toUtf8());
        }
        userFile.close();

        qDebug() << "In Mainwindow, password is altered. Current password is " << password;
        ui->statusBar->showMessage("修改密码成功", 5000);
        ui->lineEdit_password->clear();
        ui->lineEdit_username->clear();
    }
    else{
        ui->statusBar->showMessage("修改密码失败", 5000);
        ui->lineEdit_password->clear();
    }
}

void MainWindow::OnImportData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, QStringLiteral("导入数据"),"",QStringLiteral("支持格式(*.xls *.xlsx)"));
    if (filename.isEmpty()) return;

    qDebug() << "Size of database before import " << database.size();

    QXlsx::Document xlsxFile(filename);
    if (xlsxFile.load()) {  // load excel file

        int linage = xlsxFile.dimension().lastRow();  // excel文件的行数，包括了标题行。
        int col;
        QString ID;
        QString name;
        int gender;
        QString prNumber;
        int familyStatus;
        QString residentialAddress;
        QString domicile;
        QString phone;
        QString nation;
        int educationLevel;
        QString occupation;
        int militaryService;
        int maritalStatus;
        QString maritalDate;
        QString marriageNumber;
        QString spouseInfo;
        QString childrenAmount;
        QString childrenName;
        int contraception;
        int onlyChild;
        int entitledGroup;
        int entitledCategory;
        int partyMember;
        int subsistenceAllowance;
        QString allowanceAmount;
        int retirement;
        QString pension;
        int healthInsurance;
        QString immigrationDate;
        QString immigrationDistrict;
        QString emigrationDate;
        QString emigrationCause;
        QString notes;

        for(int row = 2; row <= linage; ++row){
            col = 1;
            ID = xlsxFile.read(row, 1).toString();
            name = xlsxFile.read(row, 2).toString();
            gender = xlsxFile.read(row, 3).toInt();
            prNumber = xlsxFile.read(row, 4).toString();
            familyStatus = xlsxFile.read(row, 5).toInt();
            nation = xlsxFile.read(row, 6).toString();
            residentialAddress = xlsxFile.read(row, 7).toString();
            domicile = xlsxFile.read(row, 8).toString();
            phone = xlsxFile.read(row, 9).toString();
            educationLevel = xlsxFile.read(row, 11).toInt();
            occupation = xlsxFile.read(row, 12).toString();
            militaryService = xlsxFile.read(row, 13).toInt();
            maritalStatus = xlsxFile.read(row, 14).toInt();
            maritalDate = xlsxFile.read(row, 15).toString();
            marriageNumber = xlsxFile.read(row, 16).toString();
            spouseInfo = xlsxFile.read(row, 17).toString();
            childrenAmount = xlsxFile.read(row, 18).toString();
            childrenName = xlsxFile.read(row, 19).toString();
            contraception = xlsxFile.read(row, 20).toInt();
            onlyChild = xlsxFile.read(row, 21).toInt();
            entitledGroup = xlsxFile.read(row, 22).toInt();
            entitledCategory = xlsxFile.read(row, 23).toInt();
            partyMember = xlsxFile.read(row, 24).toInt();
            subsistenceAllowance = xlsxFile.read(row, 25).toInt();
            allowanceAmount = xlsxFile.read(row, 26).toString();
            retirement = xlsxFile.read(row, 27).toInt();
            pension = xlsxFile.read(row, 28).toString();
            healthInsurance = xlsxFile.read(row, 29).toInt();
            immigrationDate = xlsxFile.read(row, 30).toString();
            immigrationDistrict = xlsxFile.read(row, 31).toString();
            emigrationDate = xlsxFile.read(row, 32).toString();
            emigrationCause = xlsxFile.read(row, 33).toString();
            notes = xlsxFile.read(row, 34).toString();

//            importedDatabase[ID] = Resident(ID, name, gender, prNumber, familyStatus, residentialAddress, domicile, phone, nation,
//                                            educationLevel, occupation, militaryService, maritalStatus, maritalDate, marriageNumber,
//                                            spouseInfo, childrenAmount, childrenName, contraception, onlyChild, entitledGroup,
//                                            entitledCategory, partyMember, subsistenceAllowance, allowanceAmount, retirement,
//                                            pension, healthInsurance, immigrationDate, immigrationDistrict, emigrationDate,
//                                            emigrationCause, notes);

            database[ID] = Resident(ID, name, gender, prNumber, familyStatus, residentialAddress, domicile, phone, nation, educationLevel,
                                    occupation, militaryService, maritalStatus, maritalDate, marriageNumber, spouseInfo, childrenAmount,
                                    childrenName, contraception, onlyChild, entitledGroup, entitledCategory, partyMember, subsistenceAllowance,
                                    allowanceAmount, retirement, pension, healthInsurance, immigrationDate, immigrationDistrict, emigrationDate,
                                    emigrationCause, notes);
        }
    }
    qDebug() << "Size of database after import " << database.size();
}

void MainWindow::OnAddData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    AddData ad(individualInformation, this);
    int ret = ad.exec();
    if(ret == QDialog::Accepted){
        individualInformation = ad.newPersonInfo;
        ui->statusBar->showMessage("增加个人资料成功", 5000);
        database[individualInformation.ID] = individualInformation;
    }
    else{
        ui->statusBar->showMessage("增加个人资料失败", 5000);
    }
}

void MainWindow::OnQueryData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    QueryData qd(database, this);
    qd.exec();
}

void MainWindow::OnExportData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    ExportData ed(database, this);
    ed.exec();
}

void MainWindow::OnLogout(){
    if(loginStatus){
        loginStatus = false;
        QJsonDocument jd;
        QJsonArray ja;

        qDebug() << "In OnLogout, the size of database is " << database.size();
        QMapIterator<QString, Resident> it(database);
        while(it.hasNext()){
            QJsonObject jo;
            it.next();
            jo.insert("ID", it.value().ID);
            jo.insert("name", it.value().name);
            jo.insert("gender", it.value().gender);
            jo.insert("prNumber", it.value().prNumber);
            jo.insert("familyStatus", it.value().familyStatus);
            jo.insert("residentialAddress", it.value().residentialAddress);
            jo.insert("domicile", it.value().domicile);
            jo.insert("phone", it.value().phone);
            jo.insert("nation", it.value().nation);
            jo.insert("educationLevel", it.value().educationLevel);
            jo.insert("occupation", it.value().occupation);
            jo.insert("militaryService", it.value().militaryService);
            jo.insert("maritalStatus", it.value().maritalStatus);
            jo.insert("maritalDate", it.value().maritalDate);
            jo.insert("marriageNumber", it.value().marriageNumber);
            jo.insert("spouseInfo", it.value().spouseInfo);
            jo.insert("childrenAmount", it.value().childrenAmount);
            jo.insert("childrenName", it.value().childrenName);
            jo.insert("contraception", it.value().contraception);
            jo.insert("onlyChild", it.value().onlyChild);
            jo.insert("entitledGroup", it.value().entitledGroup);
            jo.insert("entitledCategory", it.value().entitledCategory);
            jo.insert("partyMember", it.value().partyMember);
            jo.insert("subsistenceAllowance", it.value().subsistenceAllowance);
            jo.insert("allowanceAmount", it.value().allowanceAmount);
            jo.insert("retirement", it.value().retirement);
            jo.insert("pension", it.value().pension);
            jo.insert("healthInsurance", it.value().healthInsurance);
            jo.insert("immigrationDate", it.value().immigrationDate);
            jo.insert("immigrationDistrict", it.value().immigrationDistrict);
            jo.insert("emigrationDate", it.value().emigrationDate);
            jo.insert("emigrationCause", it.value().emigrationCause);
            jo.insert("notes", it.value().notes);

            ja.append(jo);
        }

        qDebug() << "The size of json array is " << ja.size();
        QFile file("database.json");
        if(!file.open(QIODevice::WriteOnly)) {
            qDebug() << "saveJson open error";
        } else {
            qDebug() <<"saveJson open!";
        }

        jd.setArray(ja);
        file.write(jd.toJson());
        file.close();

        ui->statusBar->showMessage("注销成功", 5000);
    }else{
        ui->statusBar->showMessage("请先登录", 5000);
    }
}

void MainWindow::OnAbout(){
    qDebug() << "in OnAbout";
    QMessageBox message(QMessageBox::NoIcon, "关于", "社区户籍人员管理系统v1.0\n作者：范翼翔\nAll Rights Reserved", QMessageBox::Ok);
    message.exec();
}

