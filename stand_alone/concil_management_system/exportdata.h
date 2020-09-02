#ifndef EXPORTDATA_H
#define EXPORTDATA_H

#include "resident.h"

#include <QMap>
#include <QDialog>

namespace Ui {
class ExportData;
}

class ExportData : public QDialog
{
    Q_OBJECT

public:
    explicit ExportData(QMap<QString, Resident> &, QWidget *parent = nullptr);
    ~ExportData();

    QMap<QString, Resident> * database;

public slots:
    void OnEnquiry();
    void OnReturn();

private:
    Ui::ExportData *ui;
    int ID2age(const QString & ID);
    bool stringCompare(QString & strStar, QString & str);
    QVector<Resident> residentArray;
    QVector<QString> resultID;
    QList<Resident> searchResult;
    int population;
    QVector<int> availableFlag;

};

#endif // EXPORTDATA_H
