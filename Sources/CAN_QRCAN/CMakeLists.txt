set(sources
  CAN_QRCAN.cpp)

set(headers
  CAN_QRCAN.h
  CAN_QRCAN_Extern.h
  CAN_QRCAN_stdafx.h)

add_library(CAN_QRCAN SHARED ${sources} ${headers})

include_directories(
  ..
  ${CMAKE_CURRENT_BINARY_DIR}/../BusEmulation/BusEmulation.dir/${CMAKE_BUILD_TYPE}/ # workaround to find BusEmulation.h
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

add_dependencies(CAN_QRCAN
  BusEmulation)

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_QRCAN PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(CAN_QRCAN
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET CAN_QRCAN
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_QRCAN> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
