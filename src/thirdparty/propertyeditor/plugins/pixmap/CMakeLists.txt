# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(PixmapProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Pixmap_SRCS pixmap.cpp  pixmapdialog.cpp)
QT4_WRAP_CPP(Pixmap_SRCS pixmapdialog.h  pixmap.h)
QT4_WRAP_UI(Pixmap_SRCS pixmapDialog.ui)

add_library(PixmapProperty SHARED ${Pixmap_SRCS})
add_dependencies(PixmapProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(PixmapProperty PropertyEditor ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY} )

install(TARGETS PixmapProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
