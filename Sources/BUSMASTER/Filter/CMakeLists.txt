set(sources
  Filter.cpp
  FilterConfigDlg.cpp
  FlexListCtrl.cpp
  MainSubListDlg.cpp
  ../Application/MultiLanguage.cpp)

set(headers
  Filter_defines.h
  Filter_extern.h
  Filter_resource.h
  Filter_stdafx.h
  FilterConfigDlg.h
  FlexListCtrl.h
  MainSubListDlg.h
  ../Application/MultiLanguage.h)

set(resources
  Filter.rc)

add_library(Filter SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_AFXEXT)

# linker options
set_target_properties(Filter PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(Filter
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET Filter
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:Filter> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
