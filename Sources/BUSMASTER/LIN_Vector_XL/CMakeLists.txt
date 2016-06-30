set(sources
  ../DIL_Interface/CommanDIL_LIN.cpp
  HardwareListing.cpp
  LIN_Vector_XL.cpp
  LIN_Vector_XL_stdafx.cpp)

set(headers
  ../DIL_Interface/CommanDIL_LIN.h
  ChangeRegDefines.h
  ChangeRegisters.h
  HardwareListing.h
  LIN_Vector_XL.h
  LIN_Vector_XL_Defines.h
  LIN_Vector_XL_Extern.h
  LIN_Vector_XL_Resource.h
  LIN_Vector_XL_stdafx.h)

set(resources
  LIN_ISOLAR_EVE_VLIN.rc)

add_library(LIN_Vector_XL SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(LIN_Vector_XL PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(LIN_Vector_XL
  Ws2_32
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET LIN_Vector_XL
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:LIN_Vector_XL> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
