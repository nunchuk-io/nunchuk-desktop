QT += qml quick
CONFIG += c++11
#QMAKE_CXXFLAGS_WARN_ON  += -Wno-unused-parameter

INCLUDEPATH +=  $$PWD/QOutlog \
                $$PWD/QQuickViewer \
                $$PWD/QQuickViewer/ScreenManager \
                $$PWD/QQuickViewer/PopupManager \
                $$PWD/QQuickViewer/Common

HEADERS += \
    $$PWD/QOutlog/QOutlog.h \
    $$PWD/QQuickViewer/Common/QAppEngine.h \
    $$PWD/QQuickViewer/QQuickViewer.h \
    $$PWD/QQuickViewer/PopupManager/QPopupManager.h \
    $$PWD/QQuickViewer/ScreenManager/QScreenManager.h \
    $$PWD/QQuickViewer/Common/QCommonDefines.h \
    $$PWD/QQuickViewer/Common/QCommonStructs.h \
    $$PWD/QQuickViewer/Common/QCommonEnums.h \
    $$PWD/QQuickViewer/ScreenManager/QScreenQueue.h

SOURCES += \
    $$PWD/QOutlog/QOutlog.cpp \
    $$PWD/QQuickViewer/QQuickViewer.cpp \
    $$PWD/QQuickViewer/PopupManager/QPopupManager.cpp \
    $$PWD/QQuickViewer/ScreenManager/QScreenManager.cpp \
    $$PWD/QQuickViewer/ScreenManager/QScreenQueue.cpp

