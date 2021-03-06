CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (leechcraft_lmp)
INCLUDE (InitLCPlugin OPTIONAL)

SET (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

IF(NOT WIN32)
	FIND_PACKAGE (Phonon)
ENDIF(NOT WIN32)

IF(WIN32)
	MESSAGE (STATUS "Using Qt's Phonon in Windows")
	SET (QT_USE_PHONON TRUE)
ELSE(WIN32)
	IF (NOT PHONON_FOUND)
		MESSAGE (WARNING "KDE Phonon not found, trying with Qt's Phonon instead...")
		SET (QT_USE_PHONON TRUE)
	ELSE (NOT PHONON_FOUND)
		MESSAGE (STATUS "Found KDE Phonon, preparing to use it instead of Qt's one...")
		SET (QT_PHONON_INCLUDES ${PHONON_INCLUDES})
		SET (QT_PHONON_LIBS ${PHONON_LIBS})
	ENDIF (NOT PHONON_FOUND)
ENDIF(WIN32)

SET (QT_USE_QTNETWORK TRUE)
INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${QT_PHONON_INCLUDES}
	${LEECHCRAFT_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
	)
SET (SRCS
	lmp.cpp
	core.cpp
	xmlsettingsmanager.cpp
	keyinterceptor.cpp
	videosettings.cpp
	entitychecker.cpp
	player.cpp
	playerwidget.cpp
	defaultwidget.cpp
	)
SET (HEADERS
	lmp.h
	core.h
	xmlsettingsmanager.h
	keyinterceptor.h
	videosettings.h
	entitychecker.h
	player.h
	playerwidget.h
	defaultwidget.h
	phonon.h
	)
SET (FORMS
    player.ui
	playerwidget.ui
	videosettings.ui
	)
SET (RESOURCES resources.qrc)
SET (TRANSLATIONS
	leechcraft_lmp_ru_RU.ts
	leechcraft_lmp_uk_UA.ts
	leechcraft_lmp_ar.ts
	leechcraft_lmp_de.ts
	leechcraft_lmp_es.ts
	leechcraft_lmp_pl.ts
	leechcraft_lmp_en.ts
	leechcraft_lmp_eo.ts
	leechcraft_lmp_fr.ts
	)
SET (COMPILED_TRANSLATIONS
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_ru_RU.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_uk_UA.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_ar.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_de.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_es.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_pl.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_en.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_eo.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lmp_fr.qm
	)

IF (NOT LC_NO_MOC)
	QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
ENDIF (NOT LC_NO_MOC)
QT4_WRAP_UI (UIS_H ${FORMS})
QT4_ADD_RESOURCES (RCCS ${RESOURCES})

ADD_CUSTOM_COMMAND (OUTPUT
	${COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${TRANSLATIONS}
	DEPENDS ${TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_LIBRARY (leechcraft_lmp SHARED
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${MOC_SRCS}
	${UIS_H}
	${RCCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_lmp
	${QT_LIBRARIES}
	${QT_PHONON_LIBS}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_lmp DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
INSTALL (FILES lmpsettings.xml DESTINATION ${LC_SETTINGS_DEST})
