include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../XProcess/xprocess.cmake)

set(XPROCESSMODULESWIDGET_SOURCES
    ${XPROCESS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogxprocessmodules.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogxprocessmodules.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogxprocessmodules.ui
    ${CMAKE_CURRENT_LIST_DIR}/xprocessmoduleswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xprocessmoduleswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xprocessmoduleswidget.ui
)
