set(sources
  J1939DBC2DBFConverter.cpp
  J1939DBC2DBFConverter_stdafx.cpp
  ../../Utility/MultiLanguageSupport.cpp)

set(headers
  J1939DBC2DBFConverter.h
  J1939DBC2DBFConverter_stdafx.h
  ../../Utility/MultiLanguageSupport.h)

add_library(J1939DBC2DBFConverter SHARED ${sources} ${headers})

add_definitions(-D_AFXEXT)

include_directories(
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

target_link_libraries(J1939DBC2DBFConverter
  DBC2DBFConverterLibrary
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

add_custom_command(
  TARGET J1939DBC2DBFConverter
  POST_BUILD
  COMMAND cmake ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND cmake ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/J1939DBC2DBFConverter.dll ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
