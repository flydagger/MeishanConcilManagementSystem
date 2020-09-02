#ifndef EXPORTDATA_H
#define EXPORTDATA_H

#include "resident.h"

#include <QMap>
#include <QDialog>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

namespace Ui {
class ExportData;
}

class ExportData : public QDialog
{
    Q_OBJECT

public:
    explicit ExportData(QWidget *parent = nullptr);
    ~ExportData();

signals:
    void send2Server(QString&);

public slots:
    void OnEnquiry();
    void OnReturn();
    void ReceiveServer(QString&);

private:
    Ui::ExportData *ui;
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

    int ID2age(const QString & ID);
    bool stringCompare(QString & strStar, QString & str);
//    QVector<Resident> residentArray;
//    QVector<QString> resultID;
    QList<Resident> searchResult;
    int population;
//    QVector<int> availableFlag;
//    bool (*SendServer)(const int& mode, const QMap<QString, Resident>&, const bool&, const QTcpSocket&, const QString&);

};

#endif // EXPORTDATA_H
