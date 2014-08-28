set(sources
  AcceptanceFilterDlg.cpp
  CAN_Kvaser_CAN.cpp
  CANRegAnalysis.cpp
  ChangeRegisters.cpp
  HardwareListing.cpp
  ../Application/MultiLanguage.cpp)

set(headers
  AcceptanceFilterDlg.h
  CAN_Kvaser_CAN.h
  CAN_Kvaser_CAN_Extern.h
  CAN_Kvaser_CAN_Resource.h
  CAN_Kvaser_CAN_stdafx.h
  ChangeRegDefines.h
  ChangeRegisters.h
  ContrConfigPeakUsbDefs.h
  HardwareListing.h
  ../Application/MultiLanguage.h)

set(resources
  CAN_Kvaser_CAN.rc)

add_library(CAN_Kvaser_CAN SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_Kvaser_CAN PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /STACK:8000000")
target_link_libraries(CAN_Kvaser_CAN
  ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/Lib/MS/canlib32.lib
  CommonClass
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET CAN_Kvaser_CAN
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_Kvaser_CAN> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/bin/canlib32.dll ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
