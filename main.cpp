#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/Fbur.png"));
    splash->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("启动Fbur..."),topRight,Qt::white);

    MainWindow w;

    splash->showMessage(QObject::tr("Fbur启动完成..."),topRight,Qt::white);

    w.show();

//    splash->finish(&w);
//    delete splash;

    return a.exec();
}
