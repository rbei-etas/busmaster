set(sources
  ListCtrlEx.cpp
  ../Application/MultiLanguage.cpp
  TreeViewEx.cpp
  TSEditorGUI.cpp
  TSEditorGUI_ChildFrame.cpp
  TSEditorGUI_PropertyView.cpp
  TSEditorGUI_SettingsDlg.cpp)

set(headers
  ListCtrlEx.h
  ../Application/MultiLanguage.h
  TreeViewEx.h
  TSEditorGUI_ChildFrame.h
  TSEditorGUI_Definitions.h
  TSEditorGUI_Extern.h
  TSEditorGUI_PropertyView.h
  TSEditorGUI_resource.h
  TSEditorGUI_SettingsDlg.h
  TSEditorGUI_stdafx.h)

set(resources
  TSEditorGUI_resource.rc)

add_library(TestSetupEditorGUI SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_AFXEXT)

# linker options
set_target_properties(TestSetupEditorGUI PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(TestSetupEditorGUI
  HTMLHELP
  DataTypes
  TestSetupEditorLib
  Utils)

# installer options
add_custom_command(
  TARGET TestSetupEditorGUI
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:TestSetupEditorGUI> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
