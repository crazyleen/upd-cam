QT += network

RESOURCES   = ipcamera.qrc
HEADERS       = imageviewer.h \
    udp_server.h
SOURCES       = imageviewer.cpp \
                main.cpp \
    udp_server.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/ipcamera
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS ipcamera.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/ipcamera
INSTALLS += target sources

OTHER_FILES +=

LIBS += -L$$PWD/libav/lib -lavcodec -lavdevice -lavfilter -lavformat -lavresample -lavutil -lswscale

INCLUDEPATH += $$PWD/lib/include
DEPENDPATH += $$PWD/lib
