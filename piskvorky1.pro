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
    resultemitersigleton.h

SOURCES += \
	main.cpp \
	sudokudialog.cpp \
    sudokutablemodel.cpp \
    sudokucontroller.cpp \
    MEAsudoku/elitelist.cpp \
    MEAsudoku/agentsudoku.cpp \
    MEAsudoku/mea.cpp \
    sudokuthread.cpp \
    resultemitersigleton.cpp

RESOURCES += \
    resources.qrc
