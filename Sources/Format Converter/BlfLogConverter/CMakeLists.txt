set(sources
  BlfLogConverter.cpp
  BlfLogConverterDLL.cpp
  ../../Utility/MultiLanguageSupport.cpp)

set(headers
  BlfLogConverter.h
  Resource.h
  ../../Utility/MultiLanguageSupport.h)

set(resources
  BlfLogConverter.rc)

add_library(BlfLogConverter SHARED ${sources} ${headers} ${resources})

add_definitions(-D_AFXEXT)

include_directories(
  ../BlfLibrary/Src
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

target_link_libraries(BlfLogConverter
  BlfLibrary
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

add_custom_command(
  TARGET BlfLogConverter
  POST_BUILD
  COMMAND cmake ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND cmake ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/BlfLogConverter.dll ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
