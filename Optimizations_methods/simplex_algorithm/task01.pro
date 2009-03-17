# -------------------------------------------------
# Project created by QtCreator 2009-03-05T01:06:15
# -------------------------------------------------
QT -= gui
TARGET = task01
CONFIG += console
CONFIG -= app_bundle
#QMAKE_CXXFLAGS += -std=c++98 -Wall -W -pedantic
INCLUDEPATH += /usr/src/boost_1_38_0
DEFINES += USING_QT_CREATOR
TEMPLATE = app
SOURCES += main.cpp
HEADERS += numeric/simplex_alg.hpp \
    numeric/numeric.hpp \
    numeric/numeric_common.hpp \
    numeric/li_vectors.hpp \
    numeric/iterator.hpp \
    numeric/invert_matrix.hpp \
    numeric/submatrix.hpp \
    numeric/subvector.hpp
