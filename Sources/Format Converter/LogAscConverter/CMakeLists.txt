set(sources
  Log_Asc_Lexer.c
  Log_Asc_Parser.c
  LogAscConverter.cpp
  LogAscConverterDLL.cpp
  Utility.cpp
  ../../Utility/MultiLanguageSupport.cpp)

set(headers
  Log_Asc_Parser.h
  LogAscConverter.h
  ../../Utility/MultiLanguageSupport.h)

set(resources
  LogAscConverter.rc)

add_library(LogAscConverter SHARED ${sources} ${headers} ${resources})

add_custom_command(OUTPUT Log_Asc_Lexer.c
  COMMAND ${FLEX_EXECUTABLE}
  ARGS -i -L -oLog_Asc_Lexer.c ${CMAKE_CURRENT_SOURCE_DIR}/Log_Asc_Lexer.l
  DEPENDS Log_Asc_Lexer.l)

add_custom_command(OUTPUT Log_Asc_Parser.c Log_Asc_Parser.h
  COMMAND ${BISON_EXECUTABLE}
  ARGS -d -l -o${CMAKE_CURRENT_BINARY_DIR}/Log_Asc_Parser.c ${CMAKE_CURRENT_SOURCE_DIR}/Log_Asc_Parser.y
  DEPENDS Log_Asc_Parser.y
  WORKING_DIRECTORY ${BISON_PATH})

add_definitions(-D_AFXEXT)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

target_link_libraries(LogAscConverter
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES} -NODEFAULTLIB:atls)

add_custom_command(
  TARGET LogAscConverter
  POST_BUILD
  COMMAND cmake ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND cmake ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/LogAscConverter.dll ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
