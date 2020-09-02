#ifndef QUERYDATA_H
#define QUERYDATA_H

#include "resident.h"
#include "queryresult.h"

#include <QDialog>

namespace Ui {
class QueryData;
}

class QueryData : public QDialog
{
    Q_OBJECT

public:
    explicit QueryData(QMap<QString, Resident> &, QWidget *parent = nullptr);
    ~QueryData();

    QMap<QString, Resident> *database;

public slots:
    void OnEnquiry();
    void OnReturn();

private:
    Ui::QueryData *ui;
};

#endif // QUERYDATA_H
