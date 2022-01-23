QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ui \
    graphics \
    graphics/components \
    models \
    utils \
    chart

SOURCES += \
    chart/chartcomponent.cpp \
    chart/chartcontroller.cpp \
    chart/chartwidget.cpp \
    config.cpp \
    graphics/circuitscene.cpp \
    graphics/components/capacitorparallel.cpp \
    graphics/components/capacitorserial.cpp \
    graphics/components/componentgraphicsitem.cpp \
    graphics/components/generator.cpp \
    graphics/components/inductanceparallel.cpp \
    graphics/components/inductanceserial.cpp \
    graphics/components/load.cpp \
    main.cpp \
    mainwindow.cpp \
    models/circuitcomponent.cpp \
    models/circuitmodel.cpp \
    models/doublelistmodel.cpp \
    models/doublespinboxdelegate.cpp \
    models/measureunitsdelegate.cpp \
    ui/aspectratiowidget.cpp \
    ui/circuitdockwidget.cpp \
    ui/frequencydialog.cpp \
    ui/impedancedialog.cpp \
    ui/initialdatadockwidget.cpp \
    ui/preferencesdialog.cpp \
    utils/metricnotion.cpp

HEADERS += \
    chart/chartcomponent.h \
    chart/chartcontroller.h \
    chart/chartwidget.h \
    config.h \
    constants.h \
    graphics/circuitscene.h \
    graphics/components/capacitorparallel.h \
    graphics/components/capacitorserial.h \
    graphics/components/componentgraphicsitem.h \
    graphics/components/generator.h \
    graphics/components/inductanceparallel.h \
    graphics/components/inductanceserial.h \
    graphics/components/load.h \
    mainwindow.h \
    models/circuitcomponent.h \
    models/circuitmodel.h \
    models/doublelistmodel.h \
    models/doublespinboxdelegate.h \
    models/measureunitsdelegate.h \
    ui/aspectratiowidget.h \
    ui/circuitdockwidget.h \
    ui/frequencydialog.h \
    ui/impedancedialog.h \
    ui/initialdatadockwidget.h \
    ui/preferencesdialog.h \
    utils/metricnotion.h

FORMS += \
    mainwindow.ui \
    ui/circuitdockwidget.ui \
    ui/frequencydialog.ui \
    ui/impedancedialog.ui \
    ui/initialdatadockwidget.ui \
    ui/preferencesdialog.ui

TRANSLATIONS += \
    translations/smith_chart_ru.ts \
    translations/smith_chart_en.ts

CONFIG += lrelease

CONFIG(debug, debug|release) {
    VARIANT = debug
} else {
    VARIANT = release
}

# copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

copyToDestDir($$PWD/config.ini, $$OUT_PWD/$${VARIANT}/)
copyToDestDir($$PWD/translations, $$OUT_PWD/$${VARIANT}/translations/)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
