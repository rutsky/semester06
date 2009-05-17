# -------------------------------------------------
# Project created by QtCreator 2009-03-05T01:06:15
# -------------------------------------------------
QT -= gui
TARGET = 2d_with_section
CONFIG += console precompile_header warn_on
CONFIG -= app_bundle

# QMAKE_CXXFLAGS += -std=c++98 -Wall -W -pedantic
QMAKE_CXXFLAGS += -Winvalid-pch

INCLUDEPATH += /usr/src/boost_1_38_0 \
    $(MY_NUMERIC_ROOT)
DEFINES += USING_QT_CREATOR

TEMPLATE = app
SOURCES += main.cpp
HEADERS += 

PRECOMPILED_HEADER = precompiled_header.h

OBJECTS_DIR = .build
MOC_DIR     = .build
RCC_DIR     = .build
