set(sources
  CAN_IXXAT_VCI.cpp
  CAN_IXXAT_VCI_stdafx.cpp
  CANControllerConfigDlg.cpp
  CANSelectDlg.cpp
  ClientBuffer.cpp
  ClientList.cpp
  DIL_CAN_IXXAT_VCI.cpp
  HardwareListing.cpp
  IxxatCanChannel.cpp
  ../Application/MultiLanguage.cpp
  vcinpldynl.cpp)

set(headers
  AllIncludes.h
  CAN_IXXAT_VCI.h
  CAN_IXXAT_VCI_Extern.h
  CAN_IXXAT_VCI_stdafx.h
  CANControllerConfigDlg.h
  CANSelectDlg.h
  ClientBuffer.h
  ClientList.h
  DIL_CAN_IXXAT_VCI.h
  HardwareListing.h
  IxxatCanChannel.h
  ../Application/MultiLanguage.h
  Resource.h
  targetver.h
  vcinpldynl.h)

set(resources
  CAN_IXXAT_VCI.rc)

add_library(CAN_IXXAT_VCI SHARED ${sources} ${headers} ${resources})

include_directories(
  EXTERNAL/inc
  ..
  ../Include
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_IXXAT_VCI PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /STACK:8000000")
target_link_libraries(CAN_IXXAT_VCI
  Utils)

# installer options
add_custom_command(
  TARGET CAN_IXXAT_VCI
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_IXXAT_VCI> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
