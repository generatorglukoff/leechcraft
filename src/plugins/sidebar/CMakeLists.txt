IF (NOT QT_USE_FILE)
	CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
	IF (COMMAND cmake_policy)
		cmake_policy (SET CMP0003 NEW)
	ENDIF (COMMAND cmake_policy)

	PROJECT (leechcraft_sidebar)

	IF (NOT CMAKE_MODULE_PATH)
		SET (CMAKE_MODULE_PATH "/usr/local/share/leechcraft/cmake;/usr/share/leechcraft/cmake")
	ENDIF (NOT CMAKE_MODULE_PATH)

	FIND_PACKAGE (Boost REQUIRED)
	FIND_PACKAGE (Qt4 REQUIRED)
	FIND_PACKAGE (LeechCraft REQUIRED)
ENDIF (NOT QT_USE_FILE)

INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIR}
	${LEECHCRAFT_INCLUDE_DIR}
	)
SET (SRCS
	sidebar.cpp
	sbwidget.cpp
	newtabactionmanager.cpp
	qlactionmanager.cpp
	openedtabmanager.cpp
	showconfigdialog.cpp
	)
SET (HEADERS
	sidebar.h
	sbwidget.h
	newtabactionmanager.h
	qlactionmanager.h
	openedtabmanager.h
	showconfigdialog.h
	)
SET (FORMS
	sbwidget.ui
	showconfigdialog.ui
	)
IF (NOT LC_NO_MOC)
	QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
ENDIF (NOT LC_NO_MOC)
QT4_WRAP_UI (UIS_H ${FORMS})

ADD_LIBRARY (leechcraft_sidebar SHARED
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${UIS_H}
	${MOC_SRCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_sidebar
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_sidebar DESTINATION ${LC_PLUGINS_DEST})
