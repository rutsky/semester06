# transportation_problem.pro
# Transportation problem QMake project file.
# Vladimir Rutsky
# 24.05.2009

# Originally generated by Qt Creator.

QT -= gui
TARGET = transportation_problem
CONFIG += console precompile_header warn_on
CONFIG -= app_bundle

# QMAKE_CXXFLAGS += -std=c++98 -Wall -W -pedantic
QMAKE_CXXFLAGS += -Winvalid-pch

INCLUDEPATH += $(MY_NUMERIC_ROOT)

TEMPLATE = app
SOURCES += main.cpp
HEADERS += 

PRECOMPILED_HEADER = precompiled_header.h

OBJECTS_DIR = .build
MOC_DIR     = .build
RCC_DIR     = .build
