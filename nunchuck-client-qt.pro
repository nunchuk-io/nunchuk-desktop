QT += quick core

CONFIG += c++11 gnu++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=  . \
                Views/Common \
                Views \
                ifaces \
                Models

SOURCES += \
    Models/AppModel.cpp \
    Models/DeviceModel.cpp \
    Models/MasterSignerModel.cpp \
    Models/SingleSignerModel.cpp \
    Models/TransactionModel.cpp \
    Models/UTXOModel.cpp \
    Models/WalletModel.cpp \
    contrib/nayuki/QrCode.cpp \
    ifaces/QRCodeItem.cpp \
    ifaces/QRGenerator.cpp \
    ifaces/bridgeifaces.cpp \
    ifaces/nunchuckstub.cpp \
    main.cpp

HEADERS += \
    Models/AppModel.h \
    Models/DeviceModel.h \
    Models/MasterSignerModel.h \
    Models/SingleSignerModel.h \
    Models/TransactionModel.h \
    Models/UTXOModel.h \
    Models/WalletModel.h \
    contrib/nayuki/QrCode.hpp \
    ifaces/QRCodeItem.h \
    ifaces/QRGenerator.h \
    contrib/nunchuck-client/include/nunchuck.h \
    ifaces/bridgeifaces.h \
    ifaces/nunchuckstub.h

include(QAppEngine/QAppEngine.pri)
include(Views/Views.pri)

unix:!macx {
    message("Configure UNIX")
    LIBS += -L$$PWD/contrib/nunchuck-client/build -lnunchuck-client-src
    LIBS += -L/usr/lib/x86_64-linux-gnu -lboost_program_options -lboost_filesystem
    LIBS += -L/usr/lib/x86_64-linux-gnu -ldl
    DEPENDPATH +=   $$PWD/contrib/nunchuck-client/include \
                    /usr/include/boost

    INCLUDEPATH +=  $$PWD/contrib/nunchuck-client/include \
                    /usr/include/boost

} else: win32 {
    message("Configure WIN")
    DEFINES += WIN32_LEAN_AND_MEAN BOOST_USE_WINDOWS_H
    DEFINES += _WIN32_WINNT=0x0600
    DEFINES += _ITERATOR_DEBUG_LEVEL=2

    LIBS +=$$PWD/contrib/nunchuck-client/build/Debug/nunchuck-client-src.lib
    LIBS +=C:/boost/lib/libboost_program_options-vc141-mt-gd-x64-1_73.lib
    LIBS +=C:/boost/lib/libboost_filesystem-vc141-mt-gd-x64-1_73.lib
    LIBS +=C:/Qt/Qt5.12.8/Tools/mingw730_64/x86_64-w64-mingw32/lib/libws2_32.a


    DEPENDPATH +=   $$PWD/contrib/nunchuck-client/include \
                    C:/boost/include/boost-1_73

    INCLUDEPATH += $$PWD/contrib/nunchuck-client/include \
                    C:/boost/include/boost-1_73

} else : macx {
    message("Configure MAC")
    _BOOST_PATH = /usr/local/Cellar/boost/1.73.0
    INCLUDEPATH += "$${_BOOST_PATH}/include/"
    LIBS += -L$${_BOOST_PATH}/lib
    LIBS += -lboost_program_options -lboost_filesystem
    INCLUDEPATH +=  $$PWD/contrib/nunchuck-client/include
    LIBS += -L$$PWD/contrib/nunchuck-client/build -lnunchuck-client-src
}


RESOURCES += qml.qrc

TRANSLATIONS += \
    nunchuck-client-qt_en_US.ts
