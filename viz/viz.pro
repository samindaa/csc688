#-------------------------------------------------
#
# Author: Sam Abeyruwan
# Course: CSC688
#
#-------------------------------------------------

QT       += core gui

release: DESTDIR = ../build

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

TARGET = viz
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x -D_GLIBCXX_USE_NANOSLEEP

SOURCES += Main.cpp \
	Plot.cpp \
	Window.cpp \
        AppThread.cpp \
	VizObject.cpp \
	VizModule.cpp \
        plot/qcustomplot.cpp \
        ../Config.cpp \
	../Framework.cpp \
        ../FrameworkDebug.cpp \
        ../SerialPort.cpp \
	../nao/RS232Module.cpp
	
HEADERS  += Plot.h \
	Window.h \
        AppThread.h \
	VizObject.h \
	VizModule.h \
	plot/qcustomplot.h \
        ../Config.h \
	../Template.h \
	../Framework.h \
	../FloatDetails.h \
        ../SerialPort.h \
	../nao/RS232Module.h \
	../nao/RS232Representation.h 

LIBS  += 

	
