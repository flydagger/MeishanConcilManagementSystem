#ifndef MAINWINDOWSERVER_H
#define MAINWINDOWSERVER_H

#include "resident.h"
#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <QTcpServer>
#include <QDate>


namespace Ui {
class MainWindowServer;
}

class MainWindowServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowServer(QWidget *parent = nullptr);
    ~MainWindowServer();
    QString username;
    QString password;
    Resident individualInformation;
    QMap<QString, Resident> database;
//    QMap<QString, Resident> importedDatabase;

public slots:
    void OnLogin();
    void OnStartServer();
    void NewClient();
    void UserCommand();
//    void WriteClient();
    void OnChangePassword();
    void OnAddData();
    void OnImportData();
    void OnQueryData();
    void OnExportData();
//    void ManageUser();
    void OnLogout();
    void OnAbout();
    void OnSaveDatabase();
    void OnBackupDatabase();
    void ClientDisconnect();

    void ClientSocketCheck();

private:
    Ui::MainWindowServer *ui;
    bool loginStatus = false;
    bool localhostFlag = false;  // true: the localhost is valid
    bool netStatus = false;  // true: the network is successfully configured.
    bool databaseStatus = false;  // true: the database has been imported.
    bool admStatus = false;

    QDate lastBackupDatabase;

//    QSet<QString> permittedIP;
    QTcpServer *tcpServer = nullptr;
    QVector<quint32> clientAllowList;
    QTcpSocket *activeClient = nullptr;  // single threading
//    QList<QString> activeClientList;  // multithreading
//    QDataStream in;

    QList<QString> validLocalhostNameList;
    QList<QHostAddress> validLocalhostIPList;
    QString localhostName;
    QHostAddress localhostIP;  // the current IP related with CONCIL MANAGEMENT SYSTEM
    QList<QHostAddress> localhostIPList;  // all IPs of the host computer
    QMap<QString, QString> userKeyMap;  // <username, key>
    QString revdMessage;
    QString sendMessage;
    int amountOfResident;
    int indexOfResident;
    int singleTransactionCount;
    QJsonDocument *residentJsonDocument;
    QJsonArray *residentJsonArray;

    bool Configuration();
    bool ImportDatabase();
    bool ChangeUserPassword();
    bool SaveDatabase();
    bool ReadUserList();
    bool WriteUserList();
    bool UpdateDatabase(const QString&);
    QMap<QString, Resident> Json2Map(const QString&);
    Resident Json2Resident(const QByteArray&);
    QByteArray SearchInDatabase(const QString&, int);
    QJsonObject Resident2QJsonObject(const Resident&);
    bool BackupDatabase(const int&);
    bool AutoBackupDatabase();
    bool StringCompare(const QString &, const QString &);
    int ID2age(const QString &);
    bool Login();
    void TransactionImportData(const QByteArray &);
    void TransactionAlterData(const QByteArray &);
    void TransactionExportData(const QByteArray &);

    void testCommunication(const QByteArray & );

//    void openExcel(QString fileName);
//    QVariant readAll(QAxObject *sheet);
//    void castVariant2ListListVariant(const QVariant &var);
};

#endif // MAINWINDOWSERVER_H
