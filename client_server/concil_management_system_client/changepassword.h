#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(const QString&, const QString&, QWidget *parent = nullptr);
    ~ChangePassword();

    QString originalPassword;
    QString username;

signals:
    void send2Server(QString&);

public slots:
    int OnChangePassword();
    void OnCancel();

private:
    Ui::ChangePassword *ui;
    QTcpSocket cpTcpSocket;
};

#endif // CHANGEPASSWORD_H
