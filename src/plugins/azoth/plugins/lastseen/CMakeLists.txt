INCLUDE_DIRECTORIES (${AZOTH_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)
SET (LASTSEEN_SRCS
	lastseen.cpp
	)
SET (LASTSEEN_HEADERS
	lastseen.h
	)
CreateTrVars ("azoth_lastseen" "en;es;ru_RU;uk_UA" LASTSEEN_TRANSLATIONS LASTSEEN_COMPILED_TRANSLATIONS)
IF (NOT LC_NO_MOC)
	QT4_WRAP_CPP (LASTSEEN_MOC_SRCS ${LASTSEEN_HEADERS})
ENDIF (NOT LC_NO_MOC)

ADD_CUSTOM_COMMAND (OUTPUT
	${LASTSEEN_COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${LASTSEEN_TRANSLATIONS}
	DEPENDS ${LASTSEEN_TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_LIBRARY (leechcraft_azoth_lastseen SHARED
	${LASTSEEN_COMPILED_TRANSLATIONS}
	${LASTSEEN_SRCS}
	${LASTSEEN_MOC_SRCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_azoth_lastseen
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_azoth_lastseen DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${LASTSEEN_COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
