set(sources
  Comment.cpp
  Converter.cpp
  DBF2DBCConverter.cpp
  DBF2DBCConverterDLL.cpp
  Message.cpp
  Parameter.cpp
  ParameterVal.cpp
  Signal.cpp
  Utility.cpp
  ValueDescriptor.cpp
  ValueTable.cpp
  ../../Utility/MultiLanguageSupport.cpp)

set(headers
  Comment.h
  Converter.h
  DBF2DBCConverter.h
  Definitions.h
  Message.h
  Parameter.h
  ParameterVal.h
  Signal.h
  SignalValue.h
  tag.h
  Utility.h
  ValueDescriptor.h
  ValueTable.h
  ../../Utility/MultiLanguageSupport.h)

set(resources
  DBF2DBCConverter.rc)

add_library(DBF2DBCConverter SHARED ${sources} ${headers} ${resources})

add_definitions(-D_AFXEXT)

include_directories(
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

target_link_libraries(DBF2DBCConverter
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

add_custom_command(
  TARGET DBF2DBCConverter
  POST_BUILD
  COMMAND cmake ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND cmake ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/DBF2DBCConverter.dll ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
