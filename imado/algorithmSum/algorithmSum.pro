QT -= gui

TEMPLATE = lib
DEFINES += ALGORITHMSUM_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithmsum.cpp

HEADERS += \
    algorithmSum_global.h \
    algorithmsum.h

INCLUDEPATH += C:\opencv\opencv-build\install\include
LIBS += C:\opencv\opencv-build\lib\libopencv_*.a

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DESTDIR =..\MyDebug
