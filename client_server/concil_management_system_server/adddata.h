#ifndef ADDDATA_H
#define ADDDATA_H

#include "resident.h"
#include <QDialog>

namespace Ui {
class AddData;
}

class AddData : public QDialog
{
    Q_OBJECT

public:
    explicit AddData(const Resident &, QWidget *parent = nullptr);
    ~AddData();

    Resident newPersonInfo;

public slots:
    void OnSubmit();
    void OnReset();
    void OnCancel();
    void OnBirthdayUpdate();

private:
    Ui::AddData *ui;
};

#endif // ADDDATA_H
