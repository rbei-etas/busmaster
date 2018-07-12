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

include_directories(
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

# linker options
target_link_libraries(DBF2DBCConverter
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

# installer options
add_custom_command(
  TARGET DBF2DBCConverter
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:DBF2DBCConverter> ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
