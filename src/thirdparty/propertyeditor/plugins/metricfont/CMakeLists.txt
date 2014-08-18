# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(FontProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Font_SRCS changefont.cpp  font.cpp  metricfontdialog.cpp)
QT4_WRAP_CPP(Font_SRCS changefont.h  font.h  metricfontdialog.h)
QT4_WRAP_UI(Font_SRCS metricfontdialog.ui)

add_library(FontProperty SHARED ${Font_SRCS})
add_dependencies(FontProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(FontProperty PropertyEditor ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY} )

install(TARGETS FontProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
