INCLUDE_DIRECTORIES (${AZOTH_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)
SET (ADIUMSTYLES_SRCS
	adiumstyles.cpp
	adiumstylesource.cpp
	packproxymodel.cpp
	)
SET (ADIUMSTYLES_HEADERS
	adiumstyles.h
	adiumstylesource.h
	packproxymodel.h
	)
SET (ADIUMSTYLES_RESOURCES
	adiumstyles.qrc
	)
IF (NOT LC_NO_MOC)
	QT4_WRAP_CPP (ADIUMSTYLES_MOC_SRCS ${ADIUMSTYLES_HEADERS})
ENDIF (NOT LC_NO_MOC)
QT4_ADD_RESOURCES (ADIUMSTYLES_RCCS ${ADIUMSTYLES_RESOURCES})

ADD_LIBRARY (leechcraft_azoth_adiumstyles SHARED
	${ADIUMSTYLES_SRCS}
	${ADIUMSTYLES_MOC_SRCS}
	${ADIUMSTYLES_RCCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_azoth_adiumstyles
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_azoth_adiumstyles DESTINATION ${LC_PLUGINS_DEST})
INSTALL (DIRECTORY share/azoth DESTINATION share/leechcraft)