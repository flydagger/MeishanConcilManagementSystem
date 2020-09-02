#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include "resident.h"

#include <QDialog>
#include <QDate>

namespace Ui {
class QueryResult;
}

class QueryResult : public QDialog
{
    Q_OBJECT

public:
    explicit QueryResult(QMap<QString, Resident> &, QString & ID, QString & phone, QString & age, QString & name, int & gender, QString & prNumber, QWidget *parent = nullptr);
    ~QueryResult();

    QMap<QString, Resident> *database;
    QVector<Resident> residentArray;
    QVector<QString> resultID;
    Resident alteredPersonInfo;

public slots:
    void OnPreviousRecord();
    void OnRearRecord();
    void OnSubmit();
    void OnReset();
    void OnCancel();

private:
    Ui::QueryResult *ui;

    bool stringCompare(QString &, QString &);
    int ID2age(const QString &);
    void display();
    int recordIndex = 0;
};

#endif // QUERYRESULT_H
