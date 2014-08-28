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

include_directories(
  ${ZLIB_INCLUDE_DIR})

# compiler options
add_definitions(-DBLFLIBRARY_EXPORTS -D_WINDLL)

# linker options
target_link_libraries(BlfLibrary
  ${ZLIB_LIBRARY})

# installer options
add_custom_command(
  TARGET BlfLibrary
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:BlfLibrary> ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
