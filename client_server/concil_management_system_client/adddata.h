#ifndef ADDDATA_H
#define ADDDATA_H

#include "resident.h"
#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

namespace Ui {
class AddData;
}

class AddData : public QDialog
{
    Q_OBJECT

public:
    explicit AddData(QWidget *parent = nullptr);
    ~AddData();

//    Resident newPersonInfo;

signals:
    void send2Server(QString&);

public slots:
    void OnSubmit();
    void OnReset();
    void OnCancel();
    void OnBirthdayUpdate();
    void ReceiveServer(QString&);

private:
    Ui::AddData *ui;
};

#endif // ADDDATA_H
