set(sources
  dllmain.cpp)

set(headers
  resource.h)

set(resources
  AdvancedUI.rc)

add_library(AdvancedUIPlugin SHARED ${sources} ${headers} ${resources})

include_directories(
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL -DADVANCEDUIPLUGIN_EXPORTS)
set(CMAKE_MFC_FLAG 0)

# linker options
set_target_properties(AdvancedUIPlugin PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")

# installer options
add_custom_command(
  TARGET AdvancedUIPlugin
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:AdvancedUIPlugin> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
