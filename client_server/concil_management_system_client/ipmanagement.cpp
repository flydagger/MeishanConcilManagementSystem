#include "ipmanagement.h"
#include "ui_ipmanagement.h"

IPmanagement::IPmanagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IPmanagement)
{
    ui->setupUi(this);
}

IPmanagement::~IPmanagement()
{
    delete ui;
}
