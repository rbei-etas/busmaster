set(sources
  Src/Kernel/BinHelper.cpp
  Src/Kernel/BlfLibrary.cpp
  Src/Kernel/ErrorManager.cpp
  Src/Kernel/Out.cpp)

set(headers
  Src/IBlfLibrary.h
  Src/Kernel/BinHelper.h
  Src/Kernel/BlfFormat.h
  Src/Kernel/BlfLibrary.h
  Src/Kernel/ErrorManager.h
  Src/Kernel/Out.h
  Src/Kernel/Strings.h)

add_library(BlfLibrary SHARED ${sources} ${headers})

add_definitions(-DBLFLIBRARY_EXPORTS -D_WINDLL)

include_directories(BlfLibrary
  ${ZLIB_INCLUDE_DIR})

target_link_libraries(BlfLibrary
  ${ZLIB_LIBRARY})

add_custom_command(
  TARGET BlfLibrary
  POST_BUILD
  COMMAND cmake ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND cmake ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/BlfLibrary.dll ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
