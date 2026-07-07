QT += core gui
QT += widgets svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += headers

SOURCES += \
    sources/Appuntamento.cpp \
    sources/Attivita.cpp \
    sources/Impegno.cpp \
    sources/Manager.cpp \
    sources/Scadenza.cpp \
    sources/cardattivita.cpp \
    sources/dettagliattivita.cpp \
    sources/formappuntamento.cpp \
    sources/formimpegno.cpp \
    sources/formscadenza.cpp \
    sources/main.cpp \
    sources/mainwindow.cpp

HEADERS += \
    headers/Appuntamento.h \
    headers/Impegno.h \
    headers/Manager.h \
    headers/Scadenza.h \
    headers/cardattivita.h \
    headers/dettagliattivita.h \
    headers/formappuntamento.h \
    headers/formimpegno.h \
    headers/formscadenza.h \
    headers/mainwindow.h \
    headers/Attivita.h

FORMS += \
    ui/cardattivita.ui \
    ui/dettagliattivita.ui \
    ui/formappuntamento.ui \
    ui/formimpegno.ui \
    ui/formscadenza.ui \
    ui/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    Dockerfile \
    icons/IconaAppuntamentoSVG.svg \
    icons/IconaHomeSVG.svg \
    icons/IconaImpegnoSVG.svg \
    icons/IconaImportaSVG.svg \
    icons/IconaSalvaSVG.svg \
    icons/IconaScadenzaSVG.svg \
    icons/IconaUrgenteSVG.svg
