set(sources
  ../Application/MultiLanguage.cpp
  SignalDefiner.cpp
  SignalDefinerDlg.cpp)

set(headers
  ../Application/MultiLanguage.h
  SignalDefiner.h
  SignalDefiner_Extern.h
  SignalDefiner_Resource.h
  SignalDefiner_stdafx.h
  SignalDefinerDlg.h)

set(resources
  SignalDefiner.rc)

add_library(SignalDefiner SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${CMAKE_CURRENT_BINARY_DIR}/../EXTERNAL/DMGraph/DMGraph.dir/${CMAKE_BUILD_TYPE}/ # workaround to find DMGraph.tlb
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

add_dependencies(SignalDefiner
  DMGraph)

# compiler options
add_definitions(-D_USRDLL -GL)
remove_definitions(-GR)

# linker options
set_target_properties(SignalDefiner PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /LTCG")

# installer options
add_custom_command(
  TARGET SignalDefiner
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:SignalDefiner> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
