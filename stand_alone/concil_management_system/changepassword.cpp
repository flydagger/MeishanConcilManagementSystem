#include "changepassword.h"
#include "ui_changepassword.h"

#include <QDebug>

ChangePassword::ChangePassword(const QString& password, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(OnChangePassword()));

    originalPassword = password;
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

int ChangePassword::OnChangePassword(){
    if(ui->lineEdit_originalPassword->text() != originalPassword
            || ui->lineEdit_newPassword->text() != ui->lineEdit_repeatPassword->text()
            || ui->lineEdit_newPassword->text().length() < 2
            || ui->lineEdit_repeatPassword->text().length() < 2){
        reject();
        return 0;
    }else{
        originalPassword = ui->lineEdit_newPassword->text();
        accept();
        return 1;
    }
}
