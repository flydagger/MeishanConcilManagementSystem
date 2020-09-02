#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "resident.h"
#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QMap>
#include <QMapIterator>
#include <QSet>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString username;
    QString password;
    Resident individualInformation;
    QMap<QString, Resident> database;
//    QMap<QString, Resident> importedDatabase;

public slots:
    void OnLogin();
    void OnChangePassword();
    void OnAddData();
    void OnImportData();
    void OnQueryData();
    void OnExportData();
    void OnLogout();
    void OnAbout();

private:
    Ui::MainWindow *ui;
    bool loginStatus = false;
    bool admStatus = false;
    QSet<QString> permittedIP;

//    void openExcel(QString fileName);
//    QVariant readAll(QAxObject *sheet);
//    void castVariant2ListListVariant(const QVariant &var);
};

#endif // MAINWINDOW_H
