set(sources
  DefConverterPage.cpp
  FormatConverterApp.cpp
  FormatConverterWnd.cpp
  MultiLanguage.cpp
  PluginManagerBase.cpp
  PluginManagerDLL.cpp
  stdafx.cpp
  ../../Utility/MultiLanguageSupport.cpp)

set(headers
  BaseConverter.h
  DefConverterPage.h
  Definitions.h
  FormatConverterApp.h
  FormatConverterWnd.h
  GettextBusmaster.h
  MultiLanguage.h
  PluginManagerBase.h
  PluginManagerDLL.h
  resource.h
  stdafx.h
  ../../Utility/MultiLanguageSupport.h)

set(resources
  FormatConverter.rc)

add_executable(FormatConverter WIN32 ${sources} ${headers} ${resources})

add_definitions()

include_directories(
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

target_link_libraries(FormatConverter
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES} shlwapi.lib)

add_custom_command(
  TARGET FormatConverter
  POST_BUILD
  COMMAND cmake ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/
  COMMAND cmake ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/FormatConverter.exe ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/)
