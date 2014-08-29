set(sources
  ConnectionDet.cpp
  DIL_J1939.cpp
  DIL_J1939_stdafx.cpp
  MonitorNode.cpp
  NetworkMgmt.cpp
  NodeConManager.cpp
  ReadCanMsg.cpp
  TransferLayer.cpp)

set(headers
  ConnectionDet.h
  DIL_J1939.h
  DIL_J1939_Extern.h
  DIL_J1939_stdafx.h
  J1939_UtilityFuncs.h
  Macros.h
  MonitorNode.h
  NetworkMgmt.h
  NodeConManager.h
  ReadCanMsg.h
  TransferLayer.h)

add_library(DIL_J1939 SHARED ${sources} ${headers})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(DIL_J1939 PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(DIL_J1939
  Utils
  DataTypes)

# installer options
add_custom_command(
  TARGET DIL_J1939
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:DIL_J1939> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
