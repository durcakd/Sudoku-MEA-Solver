cmake_minimum_required(VERSION 2.6)

SET(3DSOFTVIZ_NAME MEASudokuSolver)
PROJECT( ${3DSOFTVIZ_NAME} C CXX )


# Install destinations
SET( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install CACHE PATH "Installation Destination" FORCE)
SET( INSTALL_DES ./ CACHE PATH "Where to install to." )

# Min size release is default
if( NOT CMAKE_BUILD_TYPE )
	SET( CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "" FORCE )
endif()

if(WIN32)
	SET(GUI_TYPE WIN32)
endif(WIN32)
# Set some Apple MacOS Specific settings
if(APPLE)
	SET(GUI_TYPE MACOSX_BUNDLE)
endif(APPLE)

#define USE_DEBUG // for debuging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	MESSAGE("built with USE_DEBUG definition")
	ADD_DEFINITIONS(-DUSE_DEBUG)
	SET(GUI_TYPE "")			# console on Debug mode
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")


# ============================================
### Source files

FILE(GLOB SRC_GUI	"src/*.cpp")
FILE(GLOB SRC_MEA	"src/MEAsudoku/*.cpp")
FILE(GLOB INCL_GUI	"include/*.h")
FILE(GLOB INCL_MEA	"include/MEAsudoku/*.h")
FILE(GLOB QRC		"resources/*.qrc")
FILE(GLOB RC		"resources/*.rc")

SET(SRC		${SRC_GUI}	${SRC_MEA})
SET(INCL	${INCL_GUI} ${INCL_MEA})

#MESSAGE("SRC  ::" ${SRC})
#MESSAGE("INCL ::" ${INCL})
#MESSAGE("QRC  ::" ${QRC})
#MESSAGE("RC   ::" ${RC})

# files for moc
SET(SOURCES_H 	./include/resultemitersigleton.h
				./include/sudokudialog.h
				./include/sudokuitemdelegate.h
				./include/sudokutablemodel.h
				./include/sudokuthread.h )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}/include
					)

# ============================================
### Qt4
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
SET(QT_USE_QMAIN true)

#invoke moc
QT4_ADD_RESOURCES( SOURCES_QRC_MOC ${QRC})
QT4_WRAP_CPP( SOURCES_H_MOC ${SOURCES_H})


MESSAGE("add_executable of ::" ${3DSOFTVIZ_NAME})
SET( SRCS 	${INCL}
			${SRC}
			${SOURCES_H_MOC}
			${SOURCES_QRC_MOC})

IF(WIN32)
  ADD_EXECUTABLE(${3DSOFTVIZ_NAME} ${GUI_TYPE} ${SRCS} ${RC} )
ELSE(WIN32)
  ADD_EXECUTABLE(${3DSOFTVIZ_NAME} ${SRCS} )
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${3DSOFTVIZ_NAME}  ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY} )


# =============================================
### install

INSTALL(TARGETS ${3DSOFTVIZ_NAME}
	RUNTIME DESTINATION ${INSTALL_DES} COMPONENT Runtime )

# Include QT4 libraries on Windows
if(DEFINED QT_QMAKE_EXECUTABLE)
	SET(QTLIBLIST QtCore QtGui)
		if(MSVC)
			SET(TYPE "d")

			foreach(qtlib ${QTLIBLIST})
				if(WIN32)
					GET_FILENAME_COMPONENT(QT_DLL_PATH_tmp ${QT_QMAKE_EXECUTABLE} PATH)

					INSTALL(FILES ${QT_DLL_PATH_tmp}/${qtlib}${type}d4.dll
						DESTINATION ${INSTALL_DES}
						CONFIGURATIONS Debug
						COMPONENT Applications)
					INSTALL(FILES ${QT_DLL_PATH_tmp}/${qtlib}4.dll
						DESTINATION ${INSTALL_DES}
						CONFIGURATIONS Release
						COMPONENT Applications)
				endif(WIN32)
			endforeach(qtlib)

		endif(MSVC)
endif(DEFINED QT_QMAKE_EXECUTABLE)

