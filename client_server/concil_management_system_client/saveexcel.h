#ifndef SAVEEXCEL_H
#define SAVEEXCEL_H

#include "resident.h"
#include <QDialog>

namespace Ui {
class SaveExcel;
}

class SaveExcel : public QDialog
{
    Q_OBJECT

public:
    explicit SaveExcel(QList<Resident> &, QWidget *parent = nullptr);
    ~SaveExcel();

public slots:
    void OnSave();
    void OnCancel();

private:
    Ui::SaveExcel *ui;
    QList<Resident> *searchResult;
    int population;
};

#endif // SAVEEXCEL_H
