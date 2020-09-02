#ifndef IPMANAGEMENT_H
#define IPMANAGEMENT_H

#include <QDialog>

namespace Ui {
class IPmanagement;
}

class IPmanagement : public QDialog
{
    Q_OBJECT

public:
    explicit IPmanagement(QWidget *parent = nullptr);
    ~IPmanagement();

private:
    Ui::IPmanagement *ui;
};

#endif // IPMANAGEMENT_H
