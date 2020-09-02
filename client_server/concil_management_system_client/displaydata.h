#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "resident.h"

#include <QWidget>

namespace Ui {
class DisplayData;
}

class DisplayData : public QWidget
{
    Q_OBJECT

public:
//    explicit DisplayData(const QMap<QString, Resident> &, int &, QString &, int &, QString &, int &, int &,
//                         int &, int &, int &, int &, int &, int &, QWidget *parent = nullptr);
    explicit DisplayData(QList<Resident> &, QWidget *parent = nullptr);
    ~DisplayData();

public slots:
    void OnSave();
    void OnCancel();

private:
    Ui::DisplayData *ui;
    QList<Resident> *searchResult;
    int population;
    int ID2age(const QString & ID);
    bool stringCompare(QString & strStar, QString & str);
};

#endif // DISPLAYDATA_H
