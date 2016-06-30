set(sources
  MsgContainer_CAN.cpp
  MsgContainer_FlexRay.cpp
  MsgContainer_J1939.cpp
  MsgContainer_LIN.cpp
  PSDI_CAN.cpp)

set(headers
  MsgContainer_CAN.h
  MsgContainer_FlexRay.h
  MsgContainer_J1939.h
  MsgContainer_LIN.h
  PSDI_CAN.h
  stdafx_CAN.h
  ../Utility/Utility_Thread.h)

add_library(PSDI_CAN SHARED ${sources} ${headers})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(PSDI_CAN PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(PSDI_CAN
  CommonClass
  DataTypes
  Utils
  DIL_Interface
  UDS_Protocol)

# installer options
add_custom_command(
  TARGET PSDI_CAN
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:PSDI_CAN> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
