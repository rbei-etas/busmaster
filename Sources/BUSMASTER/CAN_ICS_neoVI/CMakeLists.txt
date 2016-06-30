set(sources
  CAN_ICS_neoVI.cpp
  CAN_ICS_neoVI_Channel.cpp
  CAN_ICS_neoVI_Network.cpp
  CAN_ICS_neoVI_stdafx.cpp
  CANRegAnalysis_CAN_ICS_neoVI.cpp
  ChangeRegisters_CAN_ICS_neoVI.cpp
  HardwareListing.cpp
  ../Application/MultiLanguage.cpp)

set(headers
  CAN_ICS_neoVI.h
  CAN_ICS_neoVI_Channel.h
  CAN_ICS_neoVI_Extern.h
  CAN_ICS_neoVI_Network.h
  CAN_ICS_neoVI_Resource.h
  CAN_ICS_neoVI_stdafx.h
  ChangeRegDefines.h
  ChangeRegisters_CAN_ICS_neoVI.h
  ContrConfigPeakUsbDefs.h
  HardwareListing.h
  ../Application/MultiLanguage.h)

set(resources
  CAN_ICS_neoVI.rc)

add_library(CAN_ICS_neoVI SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_ICS_neoVI PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(CAN_ICS_neoVI
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET CAN_ICS_neoVI
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_ICS_neoVI> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/icsneo40.dll ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/ETASneo40.dll ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
