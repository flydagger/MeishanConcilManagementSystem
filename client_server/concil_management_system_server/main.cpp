#include "mainwindowserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindowServer w;
    w.show();
    return a.exec();
}

/*
 * 获取本地主机名和IP地址
    QString localHostName = QHostInfo::localHostName();
    qDebug() << "localhostname:" << localHostName;

    QHostInfo info = QHostInfo::fromName(localHostName);
    qDebug() << "IP Address:" <<info.addresses();
*/
