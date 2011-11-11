project(DBC_2_DBF)

cmake_minimum_required(VERSION 2.8)

find_package(MFC)

set(HEADERS
    App.h
    Comment.h
    Converter.h
    ConverterDlg.h
    Message.h
    Parameter.h
    ParameterVal.h
    Signal.h
    StdAfx.h
    Utility.h
    ValueDescriptor.h
    ValueTable.h
    definitions.h
    resource.h
    tag.h
)

set(RESOURCES
    Resource.rc
)

set(SOURCES
    App.cpp
    Comment.cpp
    Converter.cpp
    ConverterDlg.cpp
    Message.cpp
    Parameter.cpp
    ParameterVal.cpp
    Signal.cpp
    StdAfx.cpp
    Utility.cpp
    ValueDescriptor.cpp
    ValueTable.cpp
)

add_definitions(-D_AFXDLL)

set(CMAKE_MFC_FLAG 2) # Use MFC in a Shared DLL

add_executable(${PROJECT_NAME} WIN32
    ${HEADERS}
    ${RESOURCES}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} Version.lib)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
