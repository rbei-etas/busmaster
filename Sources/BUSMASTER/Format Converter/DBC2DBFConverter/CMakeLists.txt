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

include_directories(
  ../DBF2DBCConverter
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

# linker options
target_link_libraries(DBC2DBFConverter
  DBC2DBFConverterLibrary
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

# installer options
add_custom_command(
  TARGET DBC2DBFConverter
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:DBC2DBFConverter> ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
