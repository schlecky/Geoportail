TEMPLATE = app
CONFIG += uitools
QT = gui core network
CONFIG += console debug
LICENSE = GPL
SOURCES = geodl.cpp \
    tile.cpp \
    mapwidget.cpp \
    geoengine.cpp
HEADERS = geodl.h \
    tile.h \
    mapwidget.h \
    geoengine.h \
    constants.h
FORMS   = mainWindow.ui
