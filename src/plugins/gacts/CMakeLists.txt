CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (leechcraft_gacts)
INCLUDE (InitLCPlugin OPTIONAL)

SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

SET(QXT_USE_QXTCORE TRUE)
SET(QXT_USE_QXTGUI TRUE)
FIND_PACKAGE (Qxt REQUIRED)

INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIR}
	${QXT_INCLUDE_DIRS}
	${LEECHCRAFT_INCLUDE_DIR}
	)
SET (SRCS
	gacts.cpp
	)
SET (HEADERS
	gacts.h
	)
IF (NOT LC_NO_MOC)
	QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
ENDIF (NOT LC_NO_MOC)

ADD_LIBRARY (leechcraft_gacts SHARED
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${MOC_SRCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_gacts
	${QT_LIBRARIES}
	${QXT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_gacts DESTINATION ${LC_PLUGINS_DEST})
