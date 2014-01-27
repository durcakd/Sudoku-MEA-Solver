cmake_minimum_required(VERSION 2.6)

SET(3DSOFTVIZ_NAME MEASudokuSolver) 
PROJECT( ${3DSOFTVIZ_NAME} )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}    
					${CMAKE_CURRENT_SOURCE_DIR} 
					${CMAKE_CURRENT_SOURCE_DIR}/include
					)

					
### Source files
FILE(GLOB SRC_GUI  "src/*.cpp")
FILE(GLOB SRC_MEA  "src/MEAsudoku/*.cpp")
FILE(GLOB_RECURSE INCL "include/*.h")
FILE(GLOB QRC "resources/*.qrc")
FILE(GLOB RC "resources/*.rc")

SET(SRC ${SRC_GUI} ${SRC_MEA})

MESSAGE("SRC  ::" ${SRC})
MESSAGE("INCL ::" ${INCL})
MESSAGE("QRC  ::" ${QRC})
MESSAGE("RC   ::" ${RC})

# files for moc
SET(SOURCES_QRC ./resources/resources.qrc)
SET(SOURCES_H 	./include/resultemitersigleton.h  
				./include/sudokudialog.h  
				./include/sudokuitemdelegate.h  
				./include/sudokutablemodel.h 
				./include/sudokuthread.h )

				
	
	
### Qt4 
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
				
#invoke moc
QT4_ADD_RESOURCES( SOURCES_QRC_MOC ${SOURCES_QRC})
QT4_WRAP_CPP( SOURCES_H_MOC ${SOURCES_H})


MESSAGE("add_executable of ::" ${3DSOFTVIZ_NAME})
SET( SRCS 	${INCL} 
			${SRC} 
			${SOURCES_H_MOC}
			${SOURCES_QRC_MOC})
IF(WIN32)
  ADD_EXECUTABLE(${3DSOFTVIZ_NAME} ${SRCS} ${RC})
ELSE(WIN32)
  ADD_EXECUTABLE(${3DSOFTVIZ_NAME} ${SRCS} )
ENDIF(WIN32)
									
TARGET_LINK_LIBRARIES(${3DSOFTVIZ_NAME}  ${QT_LIBRARIES})