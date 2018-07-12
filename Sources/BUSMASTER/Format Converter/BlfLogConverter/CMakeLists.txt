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

include_directories(
  ../BlfLibrary/Src
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

target_link_libraries(BlfLogConverter
  BlfLibrary
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

# installer options
add_custom_command(
  TARGET BlfLogConverter
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:BlfLogConverter> ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
