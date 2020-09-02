#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(const QString& , QWidget *parent = nullptr);
    ~ChangePassword();

    QString originalPassword;

public slots:
    int OnChangePassword();

private:
    Ui::ChangePassword *ui;
};

#endif // CHANGEPASSWORD_H
