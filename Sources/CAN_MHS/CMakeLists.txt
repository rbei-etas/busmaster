set(sources
  CAN_MHS.cpp
  CAN_MHS_stdafx.cpp
  ../Application/MultiLanguage.cpp)

set(headers
  CAN_MHS.h
  CAN_MHS_Extern.h
  CAN_MHS_Resource.h
  CAN_MHS_stdafx.h
  mhstcan.h
  ../Application/MultiLanguage.h)

set(resources
  CAN_MHS.rc)

add_library(CAN_MHS SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_MHS PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(CAN_MHS
  ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/mhstcan.lib
  ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/mhsbmcfg.lib
#  mhsbmcfg
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET CAN_MHS
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_MHS> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/mhstcan.dll ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
