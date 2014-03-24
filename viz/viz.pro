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

SOURCES += Main.cpp \
	Plot.cpp \
	Window.cpp \
        AppThread.cpp \
	VizObject.cpp \
	VizModule.cpp \
        plot/qcustomplot.cpp \
	../Framework.cpp \
	../nao/RS232Module.cpp \
	../nao/rs232.cpp
	
HEADERS  += Plot.h \
	Window.h \
        AppThread.h \
	VizObject.h \
	VizModule.h \
	plot/qcustomplot.h \
	../Template.h \
	../Framework.h \
	../FloatDetails.h \
	../nao/RS232Module.h \
	../nao/RS232Representation.h \
 	../nao/rs232.h 

LIBS  += 

	
