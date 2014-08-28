set(sources
  BaseLogObject.cpp
  Format/FormatMsgCAN.cpp
  Format/FormatMsgCommon.cpp
  Format/FormatMsgJ1939.cpp
  Format/FormatMsgLIN.cpp
  FrameProcessor.cpp
  FrameProcessor_CAN.cpp
  FrameProcessor_Common.cpp
  FrameProcessor_J1939.cpp
  FrameProcessor_LIN.cpp
  LogObjectCAN.cpp
  LogObjectJ1939.cpp
  LogObjectLIN.cpp)

set(headers
  BaseFrameProcessor_CAN.h
  BaseFrameProcessor_J1939.h
  BaseFrameProcessor_LIN.h
  BaseLogObject.h
  Format/FormatMsgCAN.h
  Format/FormatMsgCommon.h
  Format/FormatMsgJ1939.h
  Format/FormatMsgLIN.h
  FrameProcessor.h
  FrameProcessor_CAN.h
  FrameProcessor_Common.h
  FrameProcessor_extern.h
  FrameProcessor_J1939.h
  FrameProcessor_LIN.h
  FrameProcessor_stdafx.h
  Logger_CommonDataTypes.h
  LogObjectCAN.h
  LogObjectJ1939.h
  LogObjectLIN.h)

add_library(FrameProcessor SHARED ${sources} ${headers})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(FrameProcessor PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /STACK:8000000")
target_link_libraries(FrameProcessor
  DataTypes
  Utils
  DIL_Interface
  CommonClass)

# installer options
add_custom_command(
  TARGET FrameProcessor
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:FrameProcessor> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
