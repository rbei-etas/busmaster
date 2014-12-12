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

include_directories(
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

# linker options
target_link_libraries(FormatConverter
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES} shlwapi.lib)

# installer options
add_custom_command(
  TARGET FormatConverter
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:FormatConverter> ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/)
