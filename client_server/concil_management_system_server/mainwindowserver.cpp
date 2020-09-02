#include "mainwindowserver.h"
#include "ui_mainwindowserver.h"
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
#include <QtNetwork>

#include <QDebug>

using namespace QXlsx;


MainWindowServer::MainWindowServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowServer),
    activeClient(new QTcpSocket)
{
    ui->setupUi(this);
    qDebug() << activeClient->state();

    connect(ui->pBtn_login, SIGNAL(clicked()), this, SLOT(OnLogin()));
    connect(ui->pBtn_startServer, SIGNAL(clicked()), this, SLOT(OnStartServer()));
    connect(ui->action_changePassword, SIGNAL(triggered()), this, SLOT(OnChangePassword()));
    connect(ui->action_importData, SIGNAL(triggered()), this, SLOT(OnImportData()));
    connect(ui->action_addData, SIGNAL(triggered()), this, SLOT(OnAddData()));
    connect(ui->action_alterData, SIGNAL(triggered()), this, SLOT(OnQueryData()));
    connect(ui->action_exportData, SIGNAL(triggered()), this, SLOT(OnExportData()));
    connect(ui->action_logout, SIGNAL(triggered()), this, SLOT(OnLogout()));
    connect(ui->action_about, SIGNAL(triggered()), this, SLOT(OnAbout()));
    connect(ui->action_saveData, SIGNAL(triggered()), this, SLOT(OnSaveDatabase()));
    connect(ui->action_backupDatabase, SIGNAL(triggered()), this, SLOT(OnBackupDatabase()));

    connect(ui->pBtn_test, SIGNAL(clicked()), this, SLOT(ClientSocketCheck()));

    if(!ImportDatabase()){
        QMessageBox::warning(this, QString("警告"), QString("载入数据库失败"));
    }else{
        qDebug() << "Succeed loading database";
        databaseStatus = true;
        if(!lastBackupDatabase.isValid()){
            ui->statusBar->showMessage(QString("首次备份数据库"));
        }else{
            if(!AutoBackupDatabase()){
                QMessageBox::warning(this, QString("警告"), QString("自动备份数据库失败"));
            }
        }
    }


}

MainWindowServer::~MainWindowServer()
{
    //The ServerInfo class provides a small GUI for the server. It also creates the
    delete ui;
    if(activeClient != nullptr){
        activeClient->abort();
    }
    tcpServer->close();
}

/**
 * @brief MainWindowServer::Configuration
 *          Load local configuration:
 * @return
 */
bool MainWindowServer::Configuration(){
    // acquire network configuration
    QFile fileLocalHost("valid_local_host_name_ip.txt");
    if(!fileLocalHost.exists()){
//        QMessageBox::critical(this, "网络配置", "网络配置文件不存在");
        fileLocalHost.close();
        return false;
    }
    if(!fileLocalHost.open(QIODevice::ReadOnly | QIODevice::Text)){
//        QMessageBox::critical(this, "网络配置", "网络配置文件无法打开");
        fileLocalHost.close();
        return false;
    }

    bool nameipFlag = false;
    while (!fileLocalHost.atEnd()) {
        QByteArray line = fileLocalHost.readLine().trimmed();
        // '%' is used to seperate host name and host ip
        // host name is before the char '%'
        // host ip is after the char '%'
        if(line[0] == '%'){
            nameipFlag = true;
        }else if(!nameipFlag){
            // before '%', namely host name
            validLocalhostNameList.push_back(line);
        }else{
            // after '%', namely host ip
            QHostAddress *tmpHostAddress = new QHostAddress(QString(line));
            validLocalhostIPList.push_back(*tmpHostAddress);
        }
    }

    fileLocalHost.close();

    if(ReadUserList()){
        ui->statusBar->showMessage(QString("读取用户列表成功"), 5000);
    }else{
        ui->statusBar->showMessage(QString("读取用户列表失败"), 5000);
        return false;
    }

    // acquire local host information
    localhostName = QHostInfo::localHostName();
    localhostIPList = QHostInfo::fromName(localhostName).addresses();

    // validate localhost
    bool hostNameFlag = false;
    foreach(QString hostName, validLocalhostNameList){
        if(hostName == localhostName){
            hostNameFlag = true;
            break;
        }
    }
    bool hostIPFlag = false;

    foreach(QHostAddress ip, validLocalhostIPList){
        foreach(QHostAddress currentIP, localhostIPList){
            if(currentIP == ip){
                hostIPFlag = true;
                localhostIP = currentIP;
                goto outer;  // go out of double loop while find an appropriate IP
            }
        }
    }
    outer:
    if(hostNameFlag && hostIPFlag){
        localhostFlag = true;  // localhost is valid
    }

    // import database
    if(!databaseStatus){
        if(ImportDatabase()){
//            QMessageBox::information(this, QString("通知"), QString("成功载入数据库"));
            databaseStatus = true;
        }else{
//            QMessageBox::information(this, QString("通知"), QString("数据库载入失败"));
            return false;
        }
    }

    // configure clientAllowList
    QFile fileClientIP("client_ip.txt");
    if(!fileClientIP.exists()){
        QMessageBox::warning(this, "警告", "用户白名单文件不存在");
        fileClientIP.close();
        return false;
    }
    if(!fileClientIP.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "警告", "用户白名单无法打开");
        fileClientIP.close();
        return false;
    }

    while(!fileClientIP.atEnd()){
        QByteArray line = fileClientIP.readLine().trimmed();
        // each line is an IP
        if(!line.isEmpty()){
            quint32 tmp_client_ip = (new QHostAddress(QString(line)))->toIPv4Address();
            clientAllowList.push_back(tmp_client_ip);
        }
    }

    fileClientIP.close();
    return true;
}

bool MainWindowServer::ImportDatabase(){
    // Read database from json file
    QString val;
    QFile file;
    file.setFileName("database.json");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << QString("Fail to open database");
        return false;
    }
    val = file.readAll();
    file.close();
    database = Json2Map(val);
    qDebug() << "In function ImportDatabase, the size of database is " << database.size();
    return true;  // succeed loading database from json file
}

QMap<QString, Resident> MainWindowServer::Json2Map(const QString & str){
    QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
    QMap<QString, Resident> tmp_database;
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

        tmp_database[ID] = Resident(ID, name, gender, prNumber, familyStatus, residentialAddress, domicile, phone, nation, educationLevel,
                                occupation, militaryService, maritalStatus, maritalDate, marriageNumber, spouseInfo, childrenAmount,
                                childrenName, contraception, onlyChild, entitledGroup, entitledCategory, partyMember, subsistenceAllowance,
                                allowanceAmount, retirement, pension, healthInsurance, immigrationDate, immigrationDistrict, emigrationDate,
                                emigrationCause, notes);
    }

    qDebug() << "In Json2Map(), the size of tmp_database is" << tmp_database.size();
    return tmp_database;
}

void MainWindowServer::OnLogin(){
    if(loginStatus){
        ui->statusBar->showMessage("不要重复登录", 5000);
        return;
    }

    if(userKeyMap.contains(ui->lineEdit_username->text())
            && userKeyMap.find(ui->lineEdit_username->text())->toUtf8() == ui->lineEdit_password->text().toUtf8()){
        ui->statusBar->showMessage("登陆成功", 5000);
        loginStatus = true;
        this->username = ui->lineEdit_username->text();
        this->password = ui->lineEdit_password->text();
        // the root is logging in
        if(this->username == QString("root")){
            admStatus = true;
        }

        // import database
        if(!databaseStatus){
            if(ImportDatabase()){
                QMessageBox::information(this, QString("通知"), QString("成功载入数据库"));
                databaseStatus = true;
            }else{
                QMessageBox::information(this, QString("通知"), QString("数据库载入失败"));
            }
        }
    }else{
        ui->statusBar->showMessage(QString("登陆失败"), 5000);
    }
}

void MainWindowServer::OnStartServer(){

    if(Configuration()){
        qDebug() << QString("In OnStartServer(), succeed configuring network.");
        QMessageBox::information(this, QString("网络配置"), QString("成功配置网络"));
    }else{
        qDebug() << QString("In OnStartServer(), fail configuring network.");
        QMessageBox::information(this, QString("网络配置"), QString("网络配置失败"));
    }

    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindowServer::NewClient);
    if (!tcpServer->listen(QHostAddress::Any, 24567)) {
        QMessageBox::critical(this, QString("服务器启动失败"), QString("错误信息：%1.").arg(tcpServer->errorString()));
        return;
    }

    ui->lb_serverStatus->setText(QString("服务器已启动"));
    ui->lb_ipPort->setTextFormat(Qt::RichText);
    ui->lb_ipPort->setText(QString("IP: %1<br>Port: %2").arg(tcpServer->serverAddress().toString()).arg(tcpServer->serverPort()));
}

void MainWindowServer::NewClient(){
//    if(!(activeClient->state() == QAbstractSocket::ConnectedState)){
//        ui->statusBar->showMessage(QString("有新的链接申请被阻塞"), 5000);
//        return;
//    }

    qDebug() << "In NewClient, " << activeClient->isValid();

    if(activeClient->state() == QAbstractSocket::ConnectedState){
        qDebug() << activeClient->state() << "hello";
        return;
    }
    activeClient = tcpServer->nextPendingConnection();

    qDebug() << activeClient->state() << activeClient->peerAddress().toIPv4Address();
    // validate the incoming client
    if(clientAllowList.contains(activeClient->peerAddress().toIPv4Address())){
        qDebug() << QString("In NewClient, client address is valid.(%1)").arg(activeClient->peerAddress().toString());
        connect(activeClient, &QAbstractSocket::disconnected, this, &MainWindowServer::ClientDisconnect);
        connect(activeClient, &QIODevice::readyRead, this, &MainWindowServer::UserCommand);
        ui->statusBar->showMessage(QString("新客户链接已建立"), 5000);
    }else{
        qDebug() << QString("In NewClient, client address is invalid.(%1)").arg(activeClient->peerAddress().toString());
        ui->statusBar->showMessage(QString("新客户链接建立失败，不在允许客户列表内"), 5000);
    }
}

/**
 * @brief MainWindowServer::UserCommand
 *  Client -> Server
        login
            "2 username password"
        change password
            "3 username prep_password new_password repeat_password"
        import data
            "4 data"
        add data
            "5 data"
        alter data
            "6 0 keywords"
            "6 1 altered data"
        export report
            "7 keywords"
 */
void MainWindowServer::UserCommand(){
    revdMessage = activeClient->readAll().trimmed();
    qDebug() << revdMessage;
    unsigned short command = revdMessage[0].unicode();
    switch (command) {
    case '2':   if(Login()){
                    sendMessage = "2 1";
                    activeClient->write(sendMessage.toUtf8());
                    activeClient->flush();
                }else{
                    sendMessage = "2 0";
                    activeClient->write(sendMessage.toUtf8());
                    activeClient->flush();
                }
                break;
    case '3':   if(ChangeUserPassword()){
                    sendMessage = "3 1";
                    activeClient->write(sendMessage.toUtf8());
                    activeClient->flush();
                }else{
                    sendMessage = "3 0";
                    activeClient->write(sendMessage.toUtf8());
                    activeClient->flush();
                }
                break;
    case '4':   TransactionImportData(revdMessage.toUtf8());
                break;
    case '5':   if(UpdateDatabase(revdMessage.mid(2))){  // server receives data from client, update database with the data
                    sendMessage = "5 1";
                    activeClient->write(sendMessage.toUtf8());
                    activeClient->flush();
                }else{
                    sendMessage = "5 0";
                    activeClient->write(sendMessage.toUtf8());
                    activeClient->flush();
                }
                break;
    case '6':   TransactionAlterData(revdMessage.toUtf8());
                break;
    case '7':   TransactionExportData(revdMessage.toUtf8());
                break;
    case '8':   activeClient->abort();
//                delete activeClient;
    case '9':   ui->statusBar->showMessage(QString("收到数据 : %1").arg(revdMessage));
                break;
    default:    break;
    }
}

/**
 * @brief MainWindowServer::UpdateDatabase - Update local database with the just received data from client.
 * @param receivedMessage
 * @return
 */
bool MainWindowServer::UpdateDatabase(const QString& receivedMessage){
    // parse QString message to Json and update database
    QMap<QString, Resident> receivedData = Json2Map(receivedMessage);

    // merge received data into local database
    foreach(const QString & id, receivedData.keys()){
        database[id] = receivedData[id];
    }

    if(!SaveDatabase()){
        return false;
    }
    return true;
}

bool MainWindowServer::ChangeUserPassword(){
    // "3 username prev_pd new_pd repeat_pd"
    QStringList strList = revdMessage.split(' ');
    // remotely changing root's password is not allowed
    if(strList[1] == "root"){
        return false;
    }

    if(userKeyMap.contains(strList[1]) && userKeyMap[strList[1]] == strList[2]
            && strList[3] == strList[4]){
        userKeyMap[strList[1]] = strList[3];  // update password

        if(WriteUserList()){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void MainWindowServer::OnChangePassword(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    ChangePassword cp(password, this);
    int ret = cp.exec();
    if(ret == QDialog::Accepted){
        password = cp.originalPassword;
        userKeyMap[this->username] = password;

        if(WriteUserList()){
            ui->statusBar->showMessage(QString("修改密码成功"), 5000);
        }else{
            ui->statusBar->showMessage(QString("修改密码失败"), 5000);
        }
    }
    else{
        ui->statusBar->showMessage(QString("修改密码失败"), 5000);
    }
}

void MainWindowServer::OnImportData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, QStringLiteral("导入数据"),"",QStringLiteral("支持格式(*.xls *.xlsx)"));
    if (filename.isEmpty()){
        QMessageBox::warning(this, QString("警告"), QString("未选定文件"));
        return;
    }

    qDebug() << "Size of database before import " << database.size();

    QXlsx::Document xlsxFile(filename);
    if (xlsxFile.load()) {  // load excel file

        int linage = xlsxFile.dimension().lastRow();  // excel文件的行数，包括了标题行。
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

            database[ID] = Resident(ID, name, gender, prNumber, familyStatus, residentialAddress, domicile, phone, nation, educationLevel,
                                    occupation, militaryService, maritalStatus, maritalDate, marriageNumber, spouseInfo, childrenAmount,
                                    childrenName, contraception, onlyChild, entitledGroup, entitledCategory, partyMember, subsistenceAllowance,
                                    allowanceAmount, retirement, pension, healthInsurance, immigrationDate, immigrationDistrict, emigrationDate,
                                    emigrationCause, notes);
        }
    }
    qDebug() << "Size of database after import " << database.size();
}

void MainWindowServer::OnAddData(){
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

void MainWindowServer::OnQueryData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    QueryData qd(database, this);
    qd.exec();
}

void MainWindowServer::OnExportData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    ExportData ed(database, this);
    ed.exec();
}

void MainWindowServer::OnLogout(){
    if(loginStatus){
        // update login status
        loginStatus = false;
        if(admStatus){
            admStatus = false;
        }

        // update database
        if(!SaveDatabase()){
            QMessageBox::critical(this, QString("致命警告"), QString("数据库保存失败"));
        }

        // update user list
        if(!WriteUserList()){
            QMessageBox::warning(this, QString("警告"), QString("用户列表保存失败"));
        }

        ui->statusBar->showMessage("注销成功", 5000);
    }else{
        ui->statusBar->showMessage("请先登录", 5000);
    }
}

void MainWindowServer::OnAbout(){
    qDebug() << "in OnAbout";
    QMessageBox message(QMessageBox::NoIcon, "关于", "社区户籍人员管理系统v2.0\n作者：范翼翔\nAll Rights Reserved", QMessageBox::Ok);
    message.exec();
}

bool MainWindowServer::SaveDatabase(){
    QJsonDocument jd;
    QJsonArray ja;

    qDebug() << "In SaveDatabase, the size of database is " << database.size();

    for(Resident & i: database.values()){
        ja.append(Resident2QJsonObject(i));
    }

    qDebug() << "The size of json array is " << ja.size();
    QFile fileDatabase("database.json");
    if(!fileDatabase.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "saveJson open error";
        fileDatabase.close();
        return false;
//        QMessageBox::warning(this, QString("警告"), QString("打开数据库失败"));
    }

    jd.setArray(ja);
    if(fileDatabase.write(jd.toJson()) == -1 || !(fileDatabase.flush())){
        fileDatabase.close();
        return false;
//        QMessageBox::warning(this, QString("警告"), QString("写入数据库失败"));
    }

    fileDatabase.close();
    return true;
}

bool MainWindowServer::ReadUserList(){
    // acquire user key pair (user privilege)
    QFile fileUserList("user_list.txt");
    if(!fileUserList.exists()){
        fileUserList.close();
        return false;
    }
    if(!fileUserList.open(QIODevice::ReadOnly | QIODevice::Text)){
        fileUserList.close();
        return false;
    }

    while(!fileUserList.atEnd()){
        QByteArray line = fileUserList.readLine().trimmed();
        // each line is a username-password pair seperated by ' '
        if(!line.isEmpty()){
            QStringList userKeyPair = QString(line).split(' ');
            if(userKeyPair.size() == 2){
                userKeyMap.insert(userKeyPair[0], userKeyPair[1]);
            }
        }
    }

    fileUserList.close();
    return true;
}

bool MainWindowServer::WriteUserList(){
    // write user key pair (user privilege)
    QFile fileUserList("user_list.txt");
    if(!fileUserList.exists()){
        fileUserList.close();
        return false;
    }
    if(!fileUserList.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        fileUserList.close();
        return false;
    }

    QTextStream in(&fileUserList);
    foreach(const QString& name, userKeyMap.keys()){
        in<< name << ' ' << userKeyMap[name] <<"\n";

    }
    in.flush();
    fileUserList.close();
    return true;
}

/**
 * @brief MainWindowServer::SearchInDatabase
 * @param keywords
 * @param mode: 0 exact search - keywords is a list of ID,
 *              1 fuzzy search - keywords is a list consisting of ID, phone, age, name, gender, prNumber
 *              2 exact search - keywords is a list consisting of groupNumber, age, onlyChild, childrenAmount, maritalStatus,
 *                               entitledGroup, subsistenceAllowance, immigration, emigration, occupation, retirement, partyMember
 * @return QByteArray is transformed from QJsonDocument
 */
QByteArray MainWindowServer::SearchInDatabase(const QString& keywords, int mode = 0){
    if(residentJsonDocument != nullptr){
        delete residentJsonDocument;
    }
    if(residentJsonArray != nullptr){
        delete residentJsonArray;
    }
    residentJsonDocument = new QJsonDocument;
    residentJsonArray = new QJsonArray;
//    QJsonDocument jd;
//    QJsonArray ja;
    amountOfResident = 0;

    if(mode == 0){
        QStringList IDs = keywords.split('%');
        foreach(const QString& id, IDs){
            if(database.contains(id)){
                residentJsonArray->append(Resident2QJsonObject(database[id]));
                ++amountOfResident;
            }
        }
    }else if(mode == 1){
        QStringList tmp = keywords.split('%');
        QString ID = tmp[0];
        QString phone = tmp[1];
        int age = tmp[2].toInt();
        QString name = tmp[3];
        int gender = tmp[4].toInt();
        QString prNumber = tmp[5];

        QMapIterator<QString, Resident> it(database);
        if(database.size() > 0){
            if(StringCompare(it.key(), ID)
                    && StringCompare(it.value().phone, phone)
                    && StringCompare(it.value().name, name)
                    && StringCompare(it.value().prNumber, prNumber)
                    && (age == 0 || ID2age(it.value().ID) == age)
                    && it.value().gender == gender){
                residentJsonArray->append(Resident2QJsonObject(it.value()));
                ++amountOfResident;
            }
        }
        while(it.hasNext()){
            if(StringCompare(it.next().key(), ID)
                    && StringCompare(it.value().phone, phone)
                    && StringCompare(it.value().name, name)
                    && StringCompare(it.value().prNumber, prNumber)
                    && (age == 0 || ID2age(it.value().ID) == age)
                    && it.value().gender == gender){
                residentJsonArray->append(Resident2QJsonObject(it.value()));
                ++amountOfResident;
            }
        }
    }else if(mode == 2){
        QStringList tmp = keywords.split('%');
        int groupNumber = tmp[0].toInt();
        int age = tmp[1].toInt();
        int onlyChild = tmp[2].toInt();
        int childrenAmount = tmp[3].toInt();
        int maritalStatus = tmp[4].toInt();
        int entitledGroup = tmp[5].toInt();
        int subsistenceAllowance = tmp[6].toInt();
        int immigration = tmp[7].toInt();
        int emigration = tmp[8].toInt();
        int occupation = tmp[9].toInt();
        int retirement = tmp[10].toInt();
        int partyMember = tmp[11].toInt();
        QMapIterator<QString, Resident> it(database);
        if(database.size() > 0){
            if((groupNumber == 0 || it.value().prNumber.split("-")[1].toInt() == groupNumber)
                    && (age == 0 || age == ID2age(it.value().ID))\
                    && (onlyChild == 0 || onlyChild == it.value().onlyChild + 1)
                    && (childrenAmount == 0 || childrenAmount == it.value().childrenAmount + 1)
                    && (maritalStatus == 0 || maritalStatus == it.value().maritalStatus + 1)
                    && (entitledGroup == 0 || entitledGroup == it.value().entitledGroup + 1)
                    && (subsistenceAllowance == 0 || subsistenceAllowance == it.value().subsistenceAllowance + 1)
                    && (immigration == 0 || immigration == 1
                        || (immigration == 2 && it.value().immigrationDate == "1800-01-01")
                        || (immigration == 3 && it.value().immigrationDate != "1800-01-01"))
                    && (emigration == 0 || emigration == 1
                        || (emigration == 2 &&  it.value().emigrationDate == "1800-01-01")
                        || (emigration == 3 &&  it.value().emigrationDate != "1800-01-01"))
                    && (occupation == 0 || occupation == it.value().occupation + 1)
                    && (retirement == 0 || retirement == it.value().retirement + 1)
                    && (partyMember == 0 || partyMember == it.value().partyMember + 1)
                     ){
                residentJsonArray->append(Resident2QJsonObject(it.value()));
                ++amountOfResident;
            }
        }
        while(it.hasNext()){
            if((groupNumber == 0 || it.next().value().prNumber.split("-")[1].toInt() == groupNumber)
                    && (age == 0 || age == ID2age(it.value().ID))\
                    && (onlyChild == 0 || onlyChild == it.value().onlyChild + 1)
                    && (childrenAmount == 0 || childrenAmount == it.value().childrenAmount + 1)
                    && (maritalStatus == 0 || maritalStatus == it.value().maritalStatus + 1)
                    && (entitledGroup == 0 || entitledGroup == it.value().entitledGroup + 1)
                    && (subsistenceAllowance == 0 || subsistenceAllowance == it.value().subsistenceAllowance + 1)
                    && (immigration == 0 || immigration == 1
                        || (immigration == 2 && it.value().immigrationDate == "1800-01-01")
                        || (immigration == 3 && it.value().immigrationDate != "1800-01-01"))
                    && (emigration == 0 || emigration == 1
                        || (emigration == 2 &&  it.value().emigrationDate == "1800-01-01")
                        || (emigration == 3 &&  it.value().emigrationDate != "1800-01-01"))
                    && (occupation == 0 || occupation == it.value().occupation + 1)
                    && (retirement == 0 || retirement == it.value().retirement + 1)
                    && (partyMember == 0 || partyMember == it.value().partyMember + 1)
                     ){
                residentJsonArray->append(Resident2QJsonObject(it.value()));
                ++amountOfResident;
            }
        }
    }

    amountOfResident = residentJsonArray->size();
    residentJsonDocument->setArray(*residentJsonArray);
    return residentJsonDocument->toJson();
}

QJsonObject MainWindowServer::Resident2QJsonObject(const Resident& res){
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

/**
 * @brief QueryResult::stringCompare
 * @param str1
 * @param str2
 * @return Return true if str1 equals to str2 or either parameter is null
 */
bool MainWindowServer::StringCompare(const QString & str1, const QString & str2){
    // if either parameter is null, then return true
    if(str1.length() == 0 || str2.length() == 0){
        return true;
    }
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

int MainWindowServer::ID2age(const QString & ID){
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

/**
 * @brief MainWindowServer::OnSaveDatabase 手动保存数据库
 */
void MainWindowServer::OnSaveDatabase(){
    if(SaveDatabase()){
        ui->statusBar->showMessage(QString("数据库保存成功"), 5000);
    }else{
        ui->statusBar->showMessage(QString("数据库保存失败"), 5000);
    }
}

/**
 * @brief MainWindowServer::BackupDatabase
 * @param mode 0 automatically backup database with data in filename
 *        mode 1 manually backup database with datatime in filename
 * @return
 */
bool MainWindowServer::BackupDatabase(const int& mode = 0){
    QJsonDocument jd;
    QJsonArray ja;

    qDebug() << "In BackupDatabase, the size of database is" << database.size();
    QMapIterator<QString, Resident> it(database);
    while(it.hasNext()){
        ja.append(Resident2QJsonObject(it.next().value()));
    }

    qDebug() << "In BackupDatabase, the size of JsonArray is" << ja.size();
    QFile fileDatabase;
    if(mode == 0){
        QString date = QDateTime::currentDateTime().toString(Qt::ISODate);
        date = date.replace(':', '-');
        date = date.replace('T', ' ');
        fileDatabase.setFileName(QString("./backup/%1 database backup.json").arg(date));
    }else{
        QString date = QDateTime::currentDateTime().toString(Qt::ISODate);
        date = date.replace(':', '-');
        date = date.replace('T', ' ');
        fileDatabase.setFileName(QString("./backup/%1 database backup.json").arg(date));
    }
    if(!fileDatabase.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        fileDatabase.close();
        return false;
    }

    jd.setArray(ja);
    if(fileDatabase.write(jd.toJson()) == -1 || !(fileDatabase.flush())){
        fileDatabase.close();
        return false;
    }

    fileDatabase.close();
    return true;
}

bool MainWindowServer::AutoBackupDatabase(){
    if(lastBackupDatabase != QDate::currentDate()){
        if(!BackupDatabase(0)){
            QMessageBox::warning(this, QString("警告"), QString("自动备份数据库失败"));
            return false;
        }
    }
    return true;
}

/**
 * @brief MainWindowServer::OnBackupDatabase manually backup database
 */
void MainWindowServer::OnBackupDatabase(){
    if(BackupDatabase(1)){
        QMessageBox::information(this, QString("通知"), QString("手动备份数据库成功"));
    }else{
        QMessageBox::information(this, QString("通知"), QString("手动备份数据库失败"));
    }
}

bool MainWindowServer::Login(){
    QStringList userKey = revdMessage.split(' ');  // "2 username password"
    if(userKey.size() != 3 || !userKeyMap.contains(userKey[1])){
        return false;
    }
    if(userKeyMap[userKey[1]] == userKey[2]){
        return true;
    }else{
        return false;
    }
}

void MainWindowServer::TransactionImportData(const QByteArray & mes){
    // "4 - NumberOfResident"
    if(mes[2] == '-'){
        singleTransactionCount = 0;
        amountOfResident = mes.mid(4).toInt();
        indexOfResident = -1;
        activeClient->write(mes);
        activeClient->flush();
        return;
    }

    // "4 Resident"
    QByteArrayList mesList = mes.split(' ');
    // order is wrong, reply with the right index
    if(mesList[1].toInt() != (indexOfResident + 1)){
        if(++singleTransactionCount > 5){
            activeClient->write("4 e");
            activeClient->flush();
            singleTransactionCount = 0;
            return;
        }
        activeClient->write(QString("4 %1").arg(indexOfResident + 1).toUtf8());
        activeClient->flush();
        return;
    }

    // order is right
    ++indexOfResident;
    Resident res = Json2Resident(mesList[2]);
    database[res.ID] = res;
    SaveDatabase();
    activeClient->write(QString("4 +").toUtf8());
    activeClient->flush();
    singleTransactionCount = 0;
}

void MainWindowServer::TransactionAlterData(const QByteArray & mes){
    QByteArrayList mesList = mes.split(' ');
    if(mesList.size() == 2){
        if(mesList[1] == "+"){
            QJsonDocument jd(residentJsonArray[indexOfResident]);
            sendMessage = jd.toJson();
            sendMessage = QString("6 %1 %2").arg(indexOfResident++).arg(sendMessage);
            activeClient->write(sendMessage.toUtf8());
            activeClient->flush();
            return;
        }else{
            if(++singleTransactionCount){
                singleTransactionCount = 0;
                activeClient->write(QString("6 e").toUtf8());
                activeClient->flush();
                return;
            }
            QJsonDocument jd(residentJsonArray[mesList[2].toInt()]);
            sendMessage = jd.toJson();
            sendMessage = QString("6 %1 %2").arg(mesList[2].toInt()).arg(sendMessage);
            activeClient->write(sendMessage.toUtf8());
            activeClient->flush();
            return;
        }
    }else if(mesList.size() == 3){
        if(mesList[1] == "0"){
            singleTransactionCount = 0;
            indexOfResident = -1;
            SearchInDatabase(revdMessage.mid(4), 1);  // fuzzy search
            sendMessage = QString("6 - %1").arg(amountOfResident);
            activeClient->write(sendMessage.toUtf8());
            activeClient->flush();
            return;
        }else if(mesList[1] == "-"){
            QJsonDocument jd(residentJsonArray[++indexOfResident]);
            sendMessage = jd.toJson();
            sendMessage = QString("6 %1 %2").arg(indexOfResident).arg(sendMessage);
            activeClient->write(sendMessage.toUtf8());
            activeClient->flush();
            return;
        }else if(mesList[1] == "*"){
            Resident r = Json2Resident(mesList[2]);
            database[r.ID] = r;
            if(SaveDatabase()){
                activeClient->write(QString("6 t").toUtf8());
                activeClient->flush();
                return;
            }else{
                activeClient->write(QString("6 f").toUtf8());
                activeClient->flush();
                return;
            }
        }else{
            activeClient->write(QString("6 e").toUtf8());
            activeClient->flush();
            return;
        }
    }else{
        activeClient->write("6 e");
        activeClient->flush();
        return;
    }
}

void MainWindowServer::TransactionExportData(const QByteArray & mes){
    QByteArrayList mesList = mes.split(' ');
    if(mesList.size() == 2){
        if(mesList[1] == "+"){
            QJsonDocument jd(residentJsonArray[indexOfResident]);
            sendMessage = jd.toJson();
            sendMessage = QString("7 %1 %2").arg(++indexOfResident).arg(sendMessage);
            activeClient->write(sendMessage.toUtf8());
            activeClient->flush();
            return;
        }else{
            if(++singleTransactionCount){
                singleTransactionCount = 0;
                activeClient->write(QString("7 e").toUtf8());
                activeClient->flush();
                return;
            }
            QJsonDocument jd(residentJsonArray[mesList[2].toInt()]);
            sendMessage = jd.toJson();
            sendMessage = QString("7 %1 %2").arg(mesList[2].toInt()).arg(sendMessage);
            activeClient->write(sendMessage.toUtf8());
            activeClient->flush();
            return;
        }
    }else if(mesList.size() == 3){
        if(mesList[1] == "0"){
            singleTransactionCount = 0;
            indexOfResident = -1;
            SearchInDatabase(revdMessage.mid(4), 2);  // exact search
            sendMessage = QString("7 - %1").arg(amountOfResident);
            activeClient->write(sendMessage.toUtf8());
            activeClient->flush();
            return;
        }else if(mesList[1] == "-"){
            QJsonDocument jd(residentJsonArray[++indexOfResident]);
            sendMessage = jd.toJson();
            sendMessage = QString("7 %1 %2").arg(indexOfResident).arg(sendMessage);
            activeClient->write(sendMessage.toUtf8());
            activeClient->flush();
            return;
        }else{
            activeClient->write("7 e");
            activeClient->flush();
            return;
        }
    }else{
        activeClient->write("7 e");
        activeClient->flush();
        return;
    }
}

Resident MainWindowServer::Json2Resident(const QByteArray& mes){
    if(residentJsonDocument != nullptr){
        delete residentJsonDocument;
    }

    *residentJsonDocument = QJsonDocument::fromJson(mes);

    QString ID = (*residentJsonDocument)[0].toObject().value("ID").toString();
    QString name = (*residentJsonDocument)[0].toObject().value("name").toString();
    int gender = (*residentJsonDocument)[0].toObject().value("gender").toInt();
    QString prNumber = (*residentJsonDocument)[0].toObject().value("prNumber").toString();
    int familyStatus = (*residentJsonDocument)[0].toObject().value("familyStatus").toInt();
    QString residentialAddress = (*residentJsonDocument)[0].toObject().value("residentialAddress").toString();
    QString domicile = (*residentJsonDocument)[0].toObject().value("domicile").toString();
    QString phone = (*residentJsonDocument)[0].toObject().value("phone").toString();
    QString nation = (*residentJsonDocument)[0].toObject().value("nation").toString();
    int educationLevel = (*residentJsonDocument)[0].toObject().value("educationLevel").toInt();
    QString occupation = (*residentJsonDocument)[0].toObject().value("occupation").toString();
    int militaryService = (*residentJsonDocument)[0].toObject().value("militaryService").toInt();
    int maritalStatus = (*residentJsonDocument)[0].toObject().value("maritalStatus").toInt();
    QString maritalDate = (*residentJsonDocument)[0].toObject().value("maritalDate").toString();
    QString marriageNumber = (*residentJsonDocument)[0].toObject().value("marriageNumber").toString();
    QString spouseInfo = (*residentJsonDocument)[0].toObject().value("spouseInfo").toString();
    QString childrenAmount = (*residentJsonDocument)[0].toObject().value("childrenAmount").toString();
    QString childrenName = (*residentJsonDocument)[0].toObject().value("childrenName").toString();
    int contraception = (*residentJsonDocument)[0].toObject().value("contraception").toInt();
    int onlyChild = (*residentJsonDocument)[0].toObject().value("onlyChild").toInt();
    int entitledGroup = (*residentJsonDocument)[0].toObject().value("entitledGroup").toInt();
    int entitledCategory = (*residentJsonDocument)[0].toObject().value("entitledCategory").toInt();
    int partyMember = (*residentJsonDocument)[0].toObject().value("partyMember").toInt();
    int subsistenceAllowance = (*residentJsonDocument)[0].toObject().value("subsistenceAllowance").toInt();
    QString allowanceAmount = (*residentJsonDocument)[0].toObject().value("allowanceAmount").toString();
    int retirement = (*residentJsonDocument)[0].toObject().value("retirement").toInt();
    QString pension = (*residentJsonDocument)[0].toObject().value("pension").toString();
    int healthInsurance = (*residentJsonDocument)[0].toObject().value("healthInsurance").toInt();
    QString immigrationDate = (*residentJsonDocument)[0].toObject().value("immigrationDate").toString();
    QString immigrationDistrict = (*residentJsonDocument)[0].toObject().value("immigrationDistrict").toString();
    QString emigrationDate = (*residentJsonDocument)[0].toObject().value("emigrationDate").toString();
    QString emigrationCause = (*residentJsonDocument)[0].toObject().value("emigrationCause").toString();
    QString notes = (*residentJsonDocument)[0].toObject().value("notes").toString();

    Resident ret(ID, name, gender, prNumber, familyStatus, residentialAddress, domicile, phone, nation, educationLevel,
             occupation, militaryService, maritalStatus, maritalDate, marriageNumber, spouseInfo, childrenAmount,
             childrenName, contraception, onlyChild, entitledGroup, entitledCategory, partyMember, subsistenceAllowance,
             allowanceAmount, retirement, pension, healthInsurance, immigrationDate, immigrationDistrict, emigrationDate,
             emigrationCause, notes);

    return ret;
}

void MainWindowServer::ClientDisconnect(){
    activeClient->abort();
//    activeClient->disconnect();
//    delete activeClient;
}

void MainWindowServer::testCommunication(const QByteArray & mes){
    QJsonDocument jd = QJsonDocument::fromJson(mes);
    qDebug() << jd.toJson();
}

void MainWindowServer::ClientSocketCheck(){
    if(activeClient != nullptr)
        qDebug() << activeClient << activeClient->state() << activeClient->isValid() << activeClient->socketType();
    else
        qDebug() << activeClient->state() << activeClient->isValid() << activeClient->socketType();
}
