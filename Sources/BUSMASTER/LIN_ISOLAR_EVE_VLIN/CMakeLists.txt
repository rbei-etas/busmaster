set(sources
  EVE_LIN_Controller.cpp
  HardwareListing.cpp
  LIN_ISOLAR_EVE_VLIN.cpp
  LIN_ISOLAR_EVE_VLIN_stdafx.cpp
  SocketUDP.cpp)

set(headers
  Common.h
  EVE_LIN_Controller.h
  HardwareListing.h
  LIN_ISOLAR_EVE_VLIN.h
  LIN_ISOLAR_EVE_VLIN_Defines.h
  LIN_ISOLAR_EVE_VLIN_Extern.h
  LIN_ISOLAR_EVE_VLIN_Resource.h
  LIN_ISOLAR_EVE_VLIN_stdafx.h
  SocketUDP.h)

set(resources
  LIN_ISOLAR_EVE_VLIN.rc)

add_library(LIN_ISOLAR_EVE_VLIN SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL -DLIN_ISOLAR_EVE_VLIN_EXPORTS)

# linker options
set_target_properties(LIN_ISOLAR_EVE_VLIN PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /FORCE")
target_link_libraries(LIN_ISOLAR_EVE_VLIN
  Ws2_32
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET LIN_ISOLAR_EVE_VLIN
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:LIN_ISOLAR_EVE_VLIN> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
