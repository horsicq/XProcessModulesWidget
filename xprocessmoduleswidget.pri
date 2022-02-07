INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/xprocessmoduleswidget.ui

HEADERS += \
    $$PWD/xprocessmoduleswidget.h

SOURCES += \
    $$PWD/xprocessmoduleswidget.cpp

!contains(XCONFIG, xprocess) {
    XCONFIG += xprocess
    include($$PWD/../XProcess/xprocess.pri)
}

!contains(XCONFIG, xbinary) {
    XCONFIG += xbinary
    include($$PWD/../Formats/xbinary.pri)
}
