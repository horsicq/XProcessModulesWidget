INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogxprocessmodules.h \
    $$PWD/xprocessmoduleswidget.h

SOURCES += \
    $$PWD/dialogxprocessmodules.cpp \
    $$PWD/xprocessmoduleswidget.cpp

FORMS += \
    $$PWD/dialogxprocessmodules.ui \
    $$PWD/xprocessmoduleswidget.ui

!contains(XCONFIG, xprocess) {
    XCONFIG += xprocess
    include($$PWD/../XProcess/xprocess.pri)
}

!contains(XCONFIG, xbinary) {
    XCONFIG += xbinary
    include($$PWD/../Formats/xbinary.pri)
}
