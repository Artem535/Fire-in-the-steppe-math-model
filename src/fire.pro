TARGET = Fire

CONFIG += release c++20 stl
QT += gui printsupport widgets

DESTDIR = ../build-qmake

HEADERS = constans.hpp \
         fireitemdelegate.h \
         firetablemodel.h \
         mainwindow.h \
         qcustomplot.h \
          
SOURCES = fireitemdelegate.cpp \
          firetablemodel.cpp \
          main.cpp \
          mainwindow.cpp \
          qcustomplot.cpp 

FORMS = mainwindow.ui 

RESOURCE = res.qrc
