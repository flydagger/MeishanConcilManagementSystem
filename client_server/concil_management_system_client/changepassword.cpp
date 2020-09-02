#include "changepassword.h"
#include "ui_changepassword.h"

#include <QDebug>

ChangePassword::ChangePassword(const QString& name, const QString& prevPassword, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    connect(ui->pushButton_modify, SIGNAL(clicked()), this, SLOT(OnChangePassword()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
    originalPassword = prevPassword;
    username = name;
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

int ChangePassword::OnChangePassword(){
    QString op = ui->lineEdit_originalPassword->text().trimmed();
    QString np = ui->lineEdit_newPassword->text().trimmed();
    QString rp = ui->lineEdit_repeatPassword->text().trimmed();
    if(op != originalPassword || np != rp || np.size() < 2 || np.size() > 20){
        qDebug() << "In OnChangePassword, fail to change password";
        return 0;
    }else{
        QString tmp = tr("3 %1 %2 %3 %4").arg(username).arg(op).arg(np).arg(rp);
        qDebug() << "In OnChangePassword, new password is valid";
        emit send2Server(tmp);
        return 1;
    }
}

void ChangePassword::OnCancel(){
    this->close();
}
