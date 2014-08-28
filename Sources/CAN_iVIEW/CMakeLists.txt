set(sources
  AcceptanceFilterDlg.cpp
  CAN_i-VIEW.cpp
  CANRegAnalysis.cpp
  ChangeRegisters.cpp
  HardwareListing.cpp)

set(headers
  AcceptanceFilterDlg.h
  CAN_i-VIEW.h
  CAN_i-VIEW_Extern.h
  CAN_i-VIEW_Resource.h
  CAN_i-VIEW_stdafx.h
  ChangeRegisters.h
  ContrConfigDefs.h
  ContrConfigPeakUsbDefs.h
  VCiViewIF.h)

set(resources
  CAN_i-VIEW.rc)

add_library(CAN_i-VIEW SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_i-VIEW PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(CAN_i-VIEW
  Ws2_32
  Utils)

# installer options
add_custom_command(
  TARGET CAN_i-VIEW
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_i-VIEW> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
