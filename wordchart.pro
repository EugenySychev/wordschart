QT += quick widgets

CONFIG += c++11

SOURCES += \
        chartadapt.cpp \
        chartcalculation.cpp \
        main.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    chartadapt.h \
    chartcalculation.h
