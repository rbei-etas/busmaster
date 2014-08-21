set(sources
  DBC2DBFConverter.cpp
  DBC2DBFConverter_stdafx.cpp
  ../../Utility/MultiLanguageSupport.cpp)

set(headers
  DBC2DBFConverter.h
  DBC2DBFConverter_stdafx.h
  ../../Utility/MultiLanguageSupport.h)

set(resources
  DBC2DBFConverter.rc)

add_library(DBC2DBFConverter SHARED ${sources} ${headers} ${resources})

add_definitions(-D_AFXEXT)

include_directories(
  ../DBF2DBCConverter
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

target_link_libraries(DBC2DBFConverter
  DBC2DBFConverterLibrary
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

add_custom_command(
  TARGET DBC2DBFConverter
  POST_BUILD
  COMMAND cmake ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND cmake ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/DBC2DBFConverter.dll ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
