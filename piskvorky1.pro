QT += core gui

HEADERS += \
	sudokudialog.h \
	sudokutablemodel.h \
	sudokucontroller.h \
	constants.h \
	MEAsudoku/elitelist.h \
	MEAsudoku/agentsudoku.h \
	MEAsudoku/mea.h \
	sudokuthread.h \
	resultemitersigleton.h \
	sudokuitemdelegate.h

SOURCES += \
	main.cpp \
	sudokudialog.cpp \
	sudokutablemodel.cpp \
	sudokucontroller.cpp \
	MEAsudoku/elitelist.cpp \
	MEAsudoku/agentsudoku.cpp \
	MEAsudoku/mea.cpp \
	sudokuthread.cpp \
	resultemitersigleton.cpp \
	sudokuitemdelegate.cpp

RESOURCES += \
	resources.qrc

RC_FILE = icon.rc

