QT += core testlib
QT -= gui

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_evtol

# Define where to find headers (The parent directory)
INCLUDEPATH += ..

# Include the source code from the parent directory
# We compile the logic directly into the test executable
SOURCES += tst_evtoltests.cpp \
           ../evtolvehicle.cpp \

HEADERS += ../evtolvehicle.h
