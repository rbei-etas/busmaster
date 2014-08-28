set(sources
  CAN_STUB.cpp)

set(headers
  CAN_STUB.h
  CAN_STUB_Extern.h
  CAN_STUB_stdafx.h)

add_library(CAN_STUB SHARED ${sources} ${headers})

include_directories(
  ..
  ${CMAKE_CURRENT_BINARY_DIR}/../BusEmulation/BusEmulation.dir/${CMAKE_BUILD_TYPE}/ # workaround to find BusEmulation.h
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

add_dependencies(CAN_STUB
  BusEmulation)

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_STUB PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /STACK:8000000")
target_link_libraries(CAN_STUB
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET CAN_STUB
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_STUB> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
