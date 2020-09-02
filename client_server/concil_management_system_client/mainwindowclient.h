#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include "resident.h"
#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <QTcpSocket>
#include <QHostAddress>


namespace Ui {
class MainWindowClient;
}

class MainWindowClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowClient(QWidget *parent = nullptr);
    ~MainWindowClient();
    QString username;
    QString password;
    Resident individualInformation;
    QMap<QString, Resident> database;
//    QMap<QString, Resident> importedDatabase;

signals:
    void send2AddData(QString&);
    void send2ExportData(QString&);
    void send2QueryData(QString&);

public slots:
    void OnLogin();
    void OnChangePassword();
    void OnAddData();
    void OnImportData();
    void OnQueryData();
    void OnExportData();
    void OnLogout();
    void OnAbout();
    void ServerCommand();
//    void OnSaveDate();
//    void OnBackupDatabase();
    void OnStartClient();
    void SucceedConnecting();
//    void ResultOfChangePassword();
    void TcpChangePassword(QString&);
    void TcpAddData(QString&);
//    void TcpImportData(QString&);
    void TcpQueryData(QString&);
    void TcpExportData(QString&);

    void communicationTest();


private:
    Ui::MainWindowClient *ui;
    bool loginStatus = false;
//    bool admStatus = false;  // root is not allowed to login on client end.
    QTcpSocket * tcpSocket = nullptr;
    QHostAddress localhostIP;
    QMap<QString, QString> userKeyMap;  // <username, key>
    QString revdMessage;
    QString sendMessage;
    QJsonDocument *residentJsonDocument;
    QJsonArray *residentJsonArray;
    int indexOfResident;
    int amountOfResident;
    int singleTransactionCount;
    int clientPort = 24567;

    bool TransactionImportData(const QByteArray &);
    void TransactionAlterData(const QByteArray &);
    void TransactionExportData(const QByteArray &);

//    bool SendServer(const int& mode, const QMap<QString, Resident>&, const QTcpSocket&, const QString&);

//    void openExcel(QString fileName);
//    QVariant readAll(QAxObject *sheet);
//    void castVariant2ListListVariant(const QVariant &var);
};

#endif // MAINWINDOWCLIENT_H
