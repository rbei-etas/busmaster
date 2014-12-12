set(sources
  Asc_Log_Lexer.c
  Asc_Log_Parser.c
  AscLogConverter.cpp
  AscLogConverterDLL.cpp
  ../../Utility/MultiLanguageSupport.cpp)

set(headers
  Asc_Log_Parser.h
  AscLogConverter.h
  Resource.h
  ../../Utility/MultiLanguageSupport.h)

set(resources
  AscLogConverter.rc)

add_library(AscLogConverter SHARED ${sources} ${headers} ${resources})

add_custom_command(
  OUTPUT Asc_Log_Lexer.c
  COMMAND ${FLEX_EXECUTABLE}
  ARGS -i -L -oAsc_Log_Lexer.c ${CMAKE_CURRENT_SOURCE_DIR}/Asc_Log_Lexer.l
  DEPENDS Asc_Log_Lexer.l)

add_custom_command(
  OUTPUT Asc_Log_Parser.c Asc_Log_Parser.h
  COMMAND ${BISON_EXECUTABLE}
  ARGS -d -l -o${CMAKE_CURRENT_BINARY_DIR}/Asc_Log_Parser.c ${CMAKE_CURRENT_SOURCE_DIR}/Asc_Log_Parser.y
  DEPENDS Asc_Log_Parser.y
  WORKING_DIRECTORY ${BISON_PATH})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

# linker options
target_link_libraries(AscLogConverter
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

# installer options
add_custom_command(
  TARGET AscLogConverter
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:AscLogConverter> ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
