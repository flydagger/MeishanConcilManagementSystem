#ifndef QUERYDATA_H
#define QUERYDATA_H

#include "resident.h"
#include "queryresult.h"

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


namespace Ui {
class QueryData;
}

class QueryData : public QDialog
{
    Q_OBJECT

public:
    explicit QueryData(QWidget *parent = nullptr);
    ~QueryData();

signals:
    void send2Server(QString&);
    void send2AlterResult(QString&);

public slots:
    void OnEnquiry();
    void OnReturn();
    void ReceiveServer(QString&);
    void ReceiveAlteredResident(Resident&);

private:
    Ui::QueryData *ui;
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

    QueryResult *qr;
    QList<Resident> residentList;
    QJsonObject Resident2QJsonObject(const Resident&);


};

#endif // QUERYDATA_H
