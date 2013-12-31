#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QDateTime>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    try{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/ico/images/Fbur.png"));
    QDateTime n2=QDateTime::currentDateTime();
    QDateTime now;
    MainWindow w;
    splash->show();
    Sleep(1500);

    w.show();
    splash->finish(&w);
    delete splash;

    return a.exec();
}
    catch(importError)
    {
        QMessageBox::critical(0, "错误", "文本格式错误");
    }
    catch(std::bad_alloc)
    {
        QMessageBox::critical(0, "错误", "内存泄露");
    }

    catch(...)
    {
        QMessageBox::critical(0, "错误", "未知错误");
    }
}
