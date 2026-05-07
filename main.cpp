#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QTranslator qtr;
    if(qtr.load(QLocale::system(),"qt","_",QLibraryInfo::path(QLibraryInfo::TranslationsPath))){
        qDebug() << "ok";
        a.installTranslator(&qtr);
    }
    QTranslator qtrBase;
    if(qtrBase.load("qtbase_" + QLocale::system().name(),QLibraryInfo::path(QLibraryInfo::TranslationsPath))){
        qDebug() << "ok";
        a.installTranslator(&qtrBase);
    }

    return a.exec();
}
