#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "config.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("RadioProg");
    app.setOrganizationDomain("radioprog.ru");
    app.setApplicationName("Impedance matching Smith chart");

    QString lang = Config::instance()->getStringValue(Config::Language);

    if(lang != "en")
    {
        QString qmPath(QApplication::applicationDirPath() + "/translations");
        QTranslator *appTranslator = new QTranslator;
        appTranslator->load("smith_chart_" + lang + ".qm", qmPath);
        app.installTranslator(appTranslator);
    }

    MainWindow w;
    w.show();
    return app.exec();
}

