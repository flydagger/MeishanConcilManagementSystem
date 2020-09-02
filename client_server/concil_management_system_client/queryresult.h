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
    explicit QueryResult(QList<Resident>&, QWidget *parent = nullptr);
    ~QueryResult();

signals:
    void replyResident(Resident&);

public slots:
    void OnPreviousRecord();
    void OnRearRecord();
    void OnSubmit();
    void OnReset();
    void OnCancel();
    void AlteredResult(QString&);

private:
    Ui::QueryResult *ui;

    QList<Resident> residentList;
    Resident alteredResident;
    bool stringCompare(QString &, QString &);
    int ID2age(const QString &);
    void display();
    int recordIndex = 0;
};

#endif // QUERYRESULT_H
