set(sources
  operations_file.cpp
  ProjConfig.cpp
  projConfigManager.cpp
  ProjectConfiguration.cpp)

set(headers
  operations_file.h
  ProjConfig.h
  projConfigManager.h
  ProjectConfiguration.h
  ProjectConfiguration_extern.h
  StdAfx_ProjectConfiguration.h)

add_library(ProjectConfiguration SHARED ${sources} ${headers})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(ProjectConfiguration PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /STACK:8000000")
target_link_libraries(ProjectConfiguration
  DataTypes)

# installer options
add_custom_command(
  TARGET ProjectConfiguration
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:ProjectConfiguration> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
