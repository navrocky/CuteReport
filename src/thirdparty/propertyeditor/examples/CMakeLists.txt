
project(propertyEditorExample)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Example_SRCS example.cpp)

add_executable(propertyEditorExample ${Example_SRCS})
add_dependencies(propertyEditorExample PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )

target_link_libraries(propertyEditorExample PropertyEditor ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} )

install(TARGETS propertyEditorExample DESTINATION "${BIN_INSTALL_DIR}" RUNTIME)
