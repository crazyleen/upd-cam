QT += network

RESOURCES   = ipcamera.qrc
HEADERS       = imageviewer.h \
    udp_server.h \
    avcoder.h
SOURCES       = imageviewer.cpp \
                main.cpp \
    udp_server.cpp \
    avcoder.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/imageviewer
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS imageviewer.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/imageviewer
INSTALLS += target sources

OTHER_FILES +=
