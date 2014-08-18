# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(BrushProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Brush_SRCS brush.cpp)
QT4_WRAP_CPP(Brush_SRCS brush.h)

add_library(BrushProperty SHARED ${Brush_SRCS})
add_dependencies(BrushProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )

target_link_libraries(BrushProperty PropertyEditor ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY} )

install(TARGETS BrushProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
