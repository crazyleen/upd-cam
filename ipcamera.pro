QT += network

RESOURCES   = ipcamera.qrc
HEADERS       = imageviewer.h \
    udp_server.h \
    recorder.h
SOURCES       = imageviewer.cpp \
                main.cpp \
    udp_server.cpp \
    recorder.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/ipcamera
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS ipcamera.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/ipcamera
INSTALLS += target sources

