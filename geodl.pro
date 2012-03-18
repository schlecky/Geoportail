TEMPLATE = app
CONFIG += uitools static
QT = gui core network
LICENSE = GPL
SOURCES = geodl.cpp \
    tile.cpp \
    mapwidget.cpp \
    geoengine.cpp \
    overlay.cpp
HEADERS = geodl.h \
    tile.h \
    mapwidget.h \
    geoengine.h \
    constants.h \
    overlay.h
FORMS = mainWindow.ui
#LIBS += -static
