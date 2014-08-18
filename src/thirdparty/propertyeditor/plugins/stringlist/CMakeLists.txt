# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(StringListProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(StringList_SRCS stringlist.cpp  stringlisteditor.cpp)
QT4_WRAP_CPP(StringList_SRCS stringlisteditor.h  stringlist.h)
QT4_WRAP_UI(StringList_SRCS stringlisteditor.ui)
QT4_ADD_RESOURCES(StringList_SRCS stringlist.qrc)

add_library(StringListProperty SHARED ${StringList_SRCS})
add_dependencies(StringListProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(StringListProperty PropertyEditor ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY} )

install(TARGETS StringListProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
