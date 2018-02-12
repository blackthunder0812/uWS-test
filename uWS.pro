TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -luWS -lssl -lcrypto -lz -lpthread
