#include "mainwindowclient.h"
#include "ui_mainwindowclient.h"
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

MainWindowClient::MainWindowClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowClient),
    tcpSocket(new QTcpSocket(this))
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
    connect(ui->pBtn_client, SIGNAL(clicked()), this, SLOT(OnStartClient()));
    connect(ui->pBtn_test, SIGNAL(clicked()), this, SLOT(communicationTest()));
    connect(tcpSocket, &QIODevice::readyRead, this, &MainWindowClient::ServerCommand);
    connect(tcpSocket, &QAbstractSocket::connected, this, &MainWindowClient::SucceedConnecting);
}

MainWindowClient::~MainWindowClient()
{
    delete ui;
    tcpSocket->abort();
}

void MainWindowClient::OnLogin(){
    if(loginStatus){
        ui->statusBar->showMessage("不要重复登录", 5000);
        return;
    }
    if(!(tcpSocket->state() == QAbstractSocket::ConnectedState)){
        ui->statusBar->showMessage("服务器尚未链接，登陆失败，请先链接服务器", 5000);
        return;
    }

    username = ui->lineEdit_username->text().trimmed();
    password = ui->lineEdit_password->text().trimmed();
    sendMessage = "2 " + username + ' ' + password;
    tcpSocket->write(sendMessage.toUtf8());
    tcpSocket->flush();
}

void MainWindowClient::OnChangePassword(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    qDebug() << "In OnChangePassword," << username << password;
    ChangePassword cp(username, password, this);
    connect(&cp, SIGNAL(send2Server(QString&)), this, SLOT(TcpChangePassword(QString&)));
    cp.exec();
}

void MainWindowClient::OnImportData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, QStringLiteral("导入数据"),"",QStringLiteral("支持格式(*.xls *.xlsx)"));
    if (filename.isEmpty()) return;

    residentJsonDocument = new QJsonDocument;
    residentJsonArray = new QJsonArray;
    QXlsx::Document xlsxFile(filename);
    if (xlsxFile.load()) {  // load excel file
        int linage = xlsxFile.dimension().lastRow();  // excel文件的行数，包括了标题行。
        for(int row = 2; row <= linage; ++row){
            QJsonObject jo;
            jo.insert("ID", xlsxFile.read(row, 1).toString());
            jo.insert("name", xlsxFile.read(row, 2).toString());
            jo.insert("gender", xlsxFile.read(row, 3).toInt());
            jo.insert("prNumber", xlsxFile.read(row, 4).toString());
            jo.insert("familyStatus", xlsxFile.read(row, 5).toInt());
            jo.insert("residentialAddress", xlsxFile.read(row, 7).toString());
            jo.insert("domicile", xlsxFile.read(row, 8).toString());
            jo.insert("phone", xlsxFile.read(row, 9).toString());
            jo.insert("nation", xlsxFile.read(row, 6).toString());
            jo.insert("educationLevel", xlsxFile.read(row, 11).toInt());
            jo.insert("occupation", xlsxFile.read(row, 12).toString());
            jo.insert("militaryService", xlsxFile.read(row, 13).toInt());
            jo.insert("maritalStatus", xlsxFile.read(row, 14).toInt());
            jo.insert("maritalDate", xlsxFile.read(row, 15).toString());
            jo.insert("marriageNumber", xlsxFile.read(row, 16).toString());
            jo.insert("spouseInfo", xlsxFile.read(row, 17).toString());
            jo.insert("childrenAmount", xlsxFile.read(row, 18).toString());
            jo.insert("childrenName", xlsxFile.read(row, 19).toString());
            jo.insert("contraception", xlsxFile.read(row, 20).toInt());
            jo.insert("onlyChild", xlsxFile.read(row, 21).toInt());
            jo.insert("entitledGroup", xlsxFile.read(row, 22).toInt());
            jo.insert("entitledCategory", xlsxFile.read(row, 23).toInt());
            jo.insert("partyMember", xlsxFile.read(row, 24).toInt());
            jo.insert("subsistenceAllowance", xlsxFile.read(row, 25).toInt());
            jo.insert("allowanceAmount", xlsxFile.read(row, 26).toString());
            jo.insert("retirement", xlsxFile.read(row, 27).toInt());
            jo.insert("pension", xlsxFile.read(row, 28).toString());
            jo.insert("healthInsurance", xlsxFile.read(row, 29).toInt());
            jo.insert("immigrationDate", xlsxFile.read(row, 30).toString());
            jo.insert("immigrationDistrict", xlsxFile.read(row, 31).toString());
            jo.insert("emigrationDate", xlsxFile.read(row, 32).toString());
            jo.insert("emigrationCause", xlsxFile.read(row, 33).toString());
            jo.insert("notes", xlsxFile.read(row, 34).toString());

            residentJsonArray->append(jo);
        }
    }
    residentJsonDocument->setArray(*residentJsonArray);
    tcpSocket->write(QString("4 - %1").arg(residentJsonArray->size()).toUtf8());
    tcpSocket->flush();
}

void MainWindowClient::OnAddData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    AddData ad(this);
    connect(&ad, SIGNAL(send2Server(QString&)), this, SLOT(TcpAddData(QString&)));
    connect(this, SIGNAL(send2AddData(QString&)), &ad, SLOT(TcpAddData(QString&)));
    ad.exec();
}

void MainWindowClient::OnQueryData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    QueryData qd(this);
    connect(&qd, SIGNAL(send2Server(QString&)), this, SLOT(TcpQueryData(QString&)));
    connect(this, SIGNAL(send2QueryData(QString&)), &qd, SLOT(ReceiveServer(QString&)));
    qd.exec();
}

void MainWindowClient::OnExportData(){
    if(!loginStatus){
        ui->statusBar->showMessage("请先登录", 5000);
        return;
    }

    ExportData ed(this);
    connect(&ed, SIGNAL(send2Server(QString&)), this, SLOT(TcpExportData(QString&)));
    connect(this, SIGNAL(send2ExportData(QString&)), &ed, SLOT(ReceiveServer(QString&)));
    ed.exec();
}

void MainWindowClient::OnLogout(){
    if(loginStatus){
        loginStatus = false;
        tcpSocket->write(QString("8").toUtf8());
        tcpSocket->flush();
        if(tcpSocket->state() != QAbstractSocket::UnconnectedState){
            tcpSocket->disconnectFromHost();
        }
        ui->statusBar->showMessage("注销成功", 5000);
    }else{
        ui->statusBar->showMessage("未登录", 5000);
    }

}

void MainWindowClient::OnAbout(){
    qDebug() << "in OnAbout";
    QMessageBox message(QMessageBox::NoIcon, "关于", "社区户籍人员管理系统v2.0\n作者：范翼翔\nAll Rights Reserved", QMessageBox::Ok);
    message.exec();
}

void MainWindowClient::OnStartClient(){
    if(tcpSocket->state() == QAbstractSocket::ConnectedState){
        tcpSocket->flush();
    }

    tcpSocket->abort();
    tcpSocket->connectToHost(ui->lineEdit_ip->text(), clientPort);
    ui->statusBar->showMessage(QString("链接已建立"));
}

void MainWindowClient::SucceedConnecting(){
    ui->lb_clientStatus->setText(QString("客户端已启动"));
    QMessageBox::information(this, QString("通知"), QString("客服端已成功连接到服务端"));
}

/*
void MainWindowClient::ResultOfChangePassword(){
    revdMessage = tcpSocket->readAll().trimmed();
    unsigned short command = revdMessage[0].unicode();
    if(command == '1'){
        QMessageBox::information(this, QString("信息"), QString("登陆成功"));
        loginStatus = true;
    }else{
        QMessageBox::information(this, QString("信息"), QString("登陆失败"));
    }
}
*/

/**
 * @brief MainWindowClient::ServerCommand
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
            "6 1 altered_data"
        export report
            "7 keywords"
 */
void MainWindowClient::ServerCommand(){
    revdMessage = tcpSocket->readAll().trimmed();
    qDebug() << revdMessage;
    unsigned short command = revdMessage[0].unicode();
    switch (command) {
    case '2':   if(revdMessage[2].unicode() == '1'){
                    loginStatus = true;
                    ui->statusBar->showMessage(QString("登陆成功"), 5000);
                }else{
                    loginStatus = false;
                    ui->statusBar->showMessage(QString("登陆失败"), 5000);
                }
                break;
    case '3':   if(revdMessage[2].unicode() == '1'){
                    QMessageBox::information(this, QString("通知"), QString("修改密码成功"));
                }else{
                    QMessageBox::information(this, QString("通知"), QString("修改密码失败"));
                    ui->statusBar->showMessage(QString("修改密码失败"), 5000);
                }
                break;
    case '4':   if(TransactionImportData(revdMessage.toUtf8())){
                    QMessageBox::information(this, QString("通知"), QString("导入数据成功"));
                }else{
                    QMessageBox::information(this, QString("通知"), QString("导入数据失败"));
                }
                break;
    case '5':   emit send2AddData(revdMessage);
                break;
    case '6':   TransactionAlterData(revdMessage.toUtf8());
                break;
    case '7':   emit send2ExportData(revdMessage);
                break;
    default: break;
    }
}

/**
 * @brief MainWindowClient::TcpChangePassword, send to server
 * @param str: "2 username password"
 */
void MainWindowClient::TcpChangePassword(QString& str){
    tcpSocket->write(str.toUtf8());
    tcpSocket->flush();
}

/**
 * @brief MainWindowClient::TcpAddData, send to server
 * @param str: "3 username prep_password new_password repeat_password"
 */
void MainWindowClient::TcpAddData(QString& str){
    tcpSocket->write(str.toUtf8());
    tcpSocket->flush();
}

/*
void MainWindowClient::TcpImportData(QString& str){
    tcpSocket->write(str.toUtf8());
    tcpSocket->flush();
}
*/

/**
 * @brief MainWindowClient::TcpExportData, send to server
 * @param str: "7 keywords"
 */
void MainWindowClient::TcpExportData(QString& str){
    tcpSocket->write(str.toUtf8());
    tcpSocket->flush();
}

/**
 * @brief MainWindowClient::TcpQueryData, send to server
 * @param str: "6 0/1 keywords"
 */
void MainWindowClient::TcpQueryData(QString& str){
    tcpSocket->write(str.toUtf8());
    tcpSocket->flush();
}

bool MainWindowClient::TransactionImportData(const QByteArray & mes){
    if(mes[2] == '-'){
        indexOfResident = 0;
        QJsonDocument jd((*residentJsonArray)[indexOfResident].toObject());
        QString str = jd.toJson();
        sendMessage = QString("4 %1 %2").arg(indexOfResident++).arg(str);
        tcpSocket->write(sendMessage.toUtf8());
        tcpSocket->flush();
    }else if(mes[2] == '+'){  // index+1
        QJsonDocument jd((*residentJsonArray)[indexOfResident].toObject());
        QString str = jd.toJson();
        sendMessage = QString("4 %1 %2").arg(indexOfResident++).arg(str);
        tcpSocket->write(sendMessage.toUtf8());
        tcpSocket->flush();
    }else if(mes[2] == 'e'){
        return false;
    }else{
        indexOfResident = mes.mid(2).toInt();
        QJsonDocument jd((*residentJsonArray)[indexOfResident].toObject());
        QString str = jd.toJson();
        sendMessage = QString("4 %1 %2").arg(indexOfResident++).arg(str);
        tcpSocket->write(sendMessage.toUtf8());
        tcpSocket->flush();
    }
    return true;
}

void MainWindowClient::TransactionAlterData(const QByteArray & mes){
    QByteArrayList mesList = mes.split(' ');
    if(mesList.size() == 2){
        if(mes[2] == 't' || mes[2] == 'f'){
            sendMessage = mes;
            emit send2QueryData(sendMessage);
            return;
        }else{
            sendMessage = "6 e";  // communication error
            emit send2QueryData(sendMessage);
            return;
        }
    }else if(mesList.size() == 3){
        // 6 - AmountOfResident
        if(mesList[1] == "-"){
            if(mesList[2] == "0"){  // 0 means no required resident is found
                sendMessage = mes;
                emit send2QueryData(sendMessage);
                return;
            }else{  // some residents are found
                amountOfResident = mesList[2].toInt();
                indexOfResident = -1;
                singleTransactionCount = 0;
                if(residentJsonDocument != nullptr){
                    delete residentJsonDocument;
                }
                if(residentJsonArray != nullptr){
                    delete residentJsonArray;
                }

                residentJsonDocument = new QJsonDocument;
                *residentJsonArray = residentJsonDocument->array();
                tcpSocket->write(mes);
                tcpSocket->flush();
                return;
            }
        }else if(mesList[1].toInt() != (indexOfResident + 1)){  // order is wrong, reply with the right index
            if(++singleTransactionCount > 5){
                tcpSocket->write("6 e");
                tcpSocket->flush();
                singleTransactionCount = 0;
                return;
            }else{
                tcpSocket->write(QString("6 %1").arg(indexOfResident + 1).toUtf8());
                tcpSocket->flush();
                return;
            }
        }else if(mesList[1].toInt() != (indexOfResident + 1)){  // order is right
            singleTransactionCount = 0;
            ++indexOfResident;
            residentJsonArray->append(QJsonDocument::fromJson(mesList[2]).object());
            tcpSocket->write(QString("6 +").toUtf8());
            tcpSocket->flush();
            // all residents have been received, so send them to child window
            if(indexOfResident == amountOfResident - 1){
                residentJsonDocument->setArray(*residentJsonArray);
                sendMessage = residentJsonDocument->toJson();
                emit send2QueryData(sendMessage);
            }
        }
    }else{
        sendMessage = "6 e";  // communication error
        emit send2QueryData(sendMessage);
        return;
    }
}

void MainWindowClient::TransactionExportData(const QByteArray & mes){
    QByteArrayList mesList = mes.split(' ');
    if(mesList[1] == "-"){
        if(mesList[2] == "0"){  // 0 means no required resident is found
            sendMessage = mes;
            emit send2ExportData(sendMessage);
            return;
        }else{  // some residents are found
            amountOfResident = mesList[2].toInt();
            indexOfResident = -1;
            singleTransactionCount = 0;
            if(residentJsonDocument != nullptr){
                delete residentJsonDocument;
            }
            if(residentJsonArray != nullptr){
                delete residentJsonArray;
            }

            residentJsonDocument = new QJsonDocument;
            *residentJsonArray = residentJsonDocument->array();
            tcpSocket->write(mes);
            tcpSocket->flush();
            return;
        }
    }else if(mesList[1].toInt() != (indexOfResident + 1)){  // order is wrong, reply with the right index
        if(++singleTransactionCount > 5){
            tcpSocket->write("7 e");
            tcpSocket->flush();
            singleTransactionCount = 0;
            return;
        }else{
            tcpSocket->write(QString("7 %1").arg(indexOfResident + 1).toUtf8());
            tcpSocket->flush();
            return;
        }
    }else if(mesList[1].toInt() != (indexOfResident + 1)){  // order is right
        singleTransactionCount = 0;
        ++indexOfResident;
        residentJsonArray->append(QJsonDocument::fromJson(mesList[2]).object());
        tcpSocket->write(QString("7 +").toUtf8());
        tcpSocket->flush();
        // all residents have been received, so send them to child window
        if(indexOfResident == amountOfResident - 1){
            residentJsonDocument->setArray(*residentJsonArray);
            sendMessage = residentJsonDocument->toJson();
            emit send2ExportData(sendMessage);
        }
    }
}

void MainWindowClient::communicationTest(){

    QJsonObject jo;
    jo.insert("ID", "110222197510167414");
    jo.insert("name", "林松");
    jo.insert("gender", 1);
    jo.insert("prNumber", "13-5-70");
    jo.insert("familyStatus", 0);
    jo.insert("residentialAddress", "锦湖苑9栋2单元8号（自有190平方）/清风丽舍3栋3单元2");
    jo.insert("domicile", "domicile3116");
    jo.insert("phone", "phone3116");
    jo.insert("nation", "汉");
    jo.insert("educationLevel", 5);
    jo.insert("occupation", "区药监局");
    jo.insert("militaryService", 4);
    jo.insert("maritalStatus", 0);
    jo.insert("maritalDate", "1981-8-18");
    jo.insert("marriageNumber", "结婚证号3116");
    jo.insert("spouseInfo", "配偶信息3116");
    jo.insert("childrenAmount", "1");
    jo.insert("childrenName", "子女姓名3116");
    jo.insert("contraception", 4);
    jo.insert("onlyChild", 1);
    jo.insert("entitledGroup", 1);
    jo.insert("entitledCategory", 0);
    jo.insert("partyMember", 1);
    jo.insert("subsistenceAllowance", 1);
    jo.insert("allowanceAmount", "1000");
    jo.insert("retirement", 1);
    jo.insert("pension", "1000");
    jo.insert("healthInsurance", 1);
    jo.insert("immigrationDate", "1994-11-23");
    jo.insert("immigrationDistrict", "迁入地区3116");
    jo.insert("emigrationDate", "1962-08-20");
    jo.insert("emigrationCause", "迁出原因3116");
    jo.insert("notes", "备注3116");

    QJsonArray ja;
    ja.append(jo);
    QJsonDocument jd;
    jd.setArray(ja);
    QString tmp = jd.toJson();
    qDebug() << tmp;

    tcpSocket->write("8 " + tmp.toUtf8());
    tcpSocket->flush();
}


