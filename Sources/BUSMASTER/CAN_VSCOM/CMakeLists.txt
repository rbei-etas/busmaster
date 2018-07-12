set(sources
  can_setup.cpp
  CAN_VSCOM.cpp
  CAN_VSCOM_stdafx.cpp)

set(headers
  CAN_VSCOM.h
  CAN_VSCOM_Extern.h
  CAN_VSCOM_stdafx.h
  resource.h
  EXTERNAL/vs_can_api.h)

set(resources
  CAN_VSCOM.rc)

add_library(CAN_VSCOM SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_VSCOM PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(CAN_VSCOM
  ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/vs_can_api.lib
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET CAN_VSCOM
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_VSCOM> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/vs_can_api.dll ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
