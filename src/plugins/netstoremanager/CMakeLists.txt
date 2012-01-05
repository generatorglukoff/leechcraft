CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (leechcraft_netstoremanager)
INCLUDE (InitLCPlugin OPTIONAL)

SET (QT_USE_QTNETWORK TRUE)
INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIR}
	${LEECHCRAFT_INCLUDE_DIR}
	)
SET (SRCS
	netstoremanager.cpp
	managertab.cpp
	xmlsettingsmanager.cpp
	accountsmanager.cpp
	accountslistwidget.cpp
	)
SET (HEADERS
	netstoremanager.h
	managertab.h
	xmlsettingsmanager.h
	accountsmanager.h
	accountslistwidget.h
	)
SET (FORMS
	managertab.ui
	accountslistwidget.ui
	)
IF (NOT LC_NO_MOC)
	QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
ENDIF (NOT LC_NO_MOC)
QT4_WRAP_UI (UIS_H ${FORMS})

ADD_LIBRARY (leechcraft_netstoremanager SHARED
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${MOC_SRCS}
	${UIS_H}
	)
TARGET_LINK_LIBRARIES (leechcraft_netstoremanager
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_netstoremanager DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES netstoremanagersettings.xml DESTINATION ${LC_SETTINGS_DEST})

OPTION (ENABLE_YANDEXDISK "Build support for Yandex.Disk" ON)

IF (ENABLE_YANDEXDISK)
	ADD_SUBDIRECTORY (plugins/yandexdisk)
ENDIF (ENABLE_YANDEXDISK)