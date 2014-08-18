# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(TimeProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Time_SRCS timeprop.cpp)
QT4_WRAP_CPP(Time_SRCS timeprop.h)

add_library(TimeProperty SHARED ${Time_SRCS})
add_dependencies(TimeProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(TimeProperty PropertyEditor ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY} )

install(TARGETS TimeProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
