QT += core gui
QT += widgets svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Appuntamento.cpp \
    Attivita.cpp \
    Impegno.cpp \
    Manager.cpp \
    Scadenza.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Appuntamento.h \
    Impegno.h \
    Manager.h \
    Scadenza.h \
    mainwindow.h \
    Attivita.h

FORMS += \
    formScadenza.ui \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    icons/IconaAppuntamentoSVG.svg \
    icons/IconaHomeSVG.svg \
    icons/IconaImpegnoSVG.svg \
    icons/IconaImportaSVG.svg \
    icons/IconaSalvaSVG.svg \
    icons/IconaScadenzaSVG.svg \
    icons/IconaUrgenteSVG.svg
