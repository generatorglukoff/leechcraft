CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (leechcraft_azoth_astrality)
INCLUDE (InitLCPlugin OPTIONAL)

SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

FIND_PACKAGE (TelepathyQt4 REQUIRED)

INCLUDE_DIRECTORIES (${AZOTH_INCLUDE_DIR}
	${TELEPATHY_QT4_INCLUDE_DIR}
	${TELEPATHY_QT4_INCLUDE_DIR}/../
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${LEECHCRAFT_INCLUDE_DIR}
	${LEECHCRAFT_INCLUDE_DIR}/azoth
	)
SET (ASTRALITY_SRCS
	astrality.cpp
	cmwrapper.cpp
	protowrapper.cpp
	accountregfirstpage.cpp
	accountwrapper.cpp
	entrywrapper.cpp
	msgwrapper.cpp
	astralityutil.cpp
	)
SET (ASTRALITY_HEADERS
	astrality.h
	cmwrapper.h
	protowrapper.h
	accountregfirstpage.h
	accountwrapper.h
	entrywrapper.h
	msgwrapper.h
	astralityutil.h
	)
SET (ASTRALITY_FORMS
	accountregfirstpage.ui
	)
SET (ASTRALITY_RESOURCES azothastralityresources.qrc)
#CreateTrVars ("azoth_astrality" "de;en;fr;it;ru_RU;uk_UA" ASTRALITY_TRANSLATIONS ASTRALITY_COMPILED_TRANSLATIONS)

IF (NOT LC_NO_MOC)
	QT4_WRAP_CPP (ASTRALITY_MOC_SRCS ${ASTRALITY_HEADERS})
ENDIF (NOT LC_NO_MOC)
QT4_WRAP_UI (ASTRALITY_UIS_H ${ASTRALITY_FORMS})
QT4_ADD_RESOURCES (ASTRALITY_RCCS ${ASTRALITY_RESOURCES})

#ADD_CUSTOM_COMMAND (OUTPUT
	#${ASTRALITY_COMPILED_TRANSLATIONS}
	#COMMAND "${QT_LRELEASE_EXECUTABLE}" ${ASTRALITY_TRANSLATIONS}
	#DEPENDS ${ASTRALITY_TRANSLATIONS}
	#WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	#)
ADD_LIBRARY (leechcraft_azoth_astrality SHARED
	${ASTRALITY_COMPILED_TRANSLATIONS}
	${ASTRALITY_SRCS}
	${ASTRALITY_MOC_SRCS}
	${ASTRALITY_UIS_H}
	${ASTRALITY_RCCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_azoth_astrality
	${QT_LIBRARIES}
	${TELEPATHY_QT4_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_azoth_astrality DESTINATION ${LC_PLUGINS_DEST})
#INSTALL (FILES ${ASTRALITY_COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
#INSTALL (FILES azothastralitysettings.xml DESTINATION ${LC_SETTINGS_DEST})
