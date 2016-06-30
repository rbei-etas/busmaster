set(sources
  stdafx.cpp
  UDS_NegRespMng.cpp
  UDS_Protocol.cpp
  UDS_TimmingWnd.cpp
  UDSMainWnd.cpp
  UDSSettingsWnd.cpp)

set(headers
  resource.h
  stdafx.h
  UDS_Extern.h
  UDS_NegRespMng.h
  UDS_Protocol.h
  UDS_Resource.h
  UDS_TimmingWnd.h
  UDSMainWnd.h
  UDSSettingsWnd.h
  UDSWnd_Defines.h)

set(resources
  uds.rc)

add_library(UDS_Protocol SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_AFXEXT)

# linker options
target_link_libraries(UDS_Protocol
  Winmm
  DIL_Interface
  Utils
  DataTypes)

# installer options
add_custom_command(
  TARGET UDS_Protocol
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:UDS_Protocol> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
