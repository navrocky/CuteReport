# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(SizefProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Sizef_SRCS sizef.cpp)
QT4_WRAP_CPP(Sizef_SRCS sizef.h)

add_library(SizefProperty SHARED ${Sizef_SRCS})
add_dependencies(SizefProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(SizefProperty PropertyEditor ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY} )

install(TARGETS SizefProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
