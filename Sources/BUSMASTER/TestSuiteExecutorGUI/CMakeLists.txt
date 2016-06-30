set(sources
  ExpressionExecutor.cpp
  lexer.c
  ../Application/MultiLanguage.cpp
  parser.c
  ResultView.cpp
  TreeViewEx.cpp
  TSExecutionCAN.cpp
  TSExecutorBase.cpp
  TSExecutorGUI.cpp
  TSExecutorGUI_ChildFrame.cpp
  TSExecutorLIB.cpp
  TSExecutorResult.cpp)

set(headers
  ExpressionExecutor.h
  ../Application/MultiLanguage.h
  parser.h
  ResultView.h
  TreeViewEx.h
  TSExecutionCAN.h
  TSExecutorBase.h
  TSExecutorGUI_ChildFrame.h
  TSExecutorGUI_Extern.h
  TSExecutorGUI_Resource.h
  TSExecutorGUI_stdafx.h
  TSExecutorLIB.h
  TSExecutorResult.h)

set(resources
  TestSuiteExecutorGUI.rc)

add_library(TestSuiteExecutorGUI SHARED ${sources} ${headers} ${resources})

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

add_custom_command(
  OUTPUT lexer.c
  COMMAND ${FLEX_EXECUTABLE}
  ARGS -i -L -olexer.c ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l
  DEPENDS lexer.l)

add_custom_command(
  OUTPUT parser.c parser.h
  COMMAND ${BISON_EXECUTABLE}
  ARGS -d -l -o${CMAKE_CURRENT_BINARY_DIR}/parser.c ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
  DEPENDS parser.y
  WORKING_DIRECTORY ${BISON_PATH})

# compiler options
add_definitions(-D_AFXEXT)

# linker options
set_target_properties(TestSuiteExecutorGUI PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(TestSuiteExecutorGUI
  DataTypes
  TestSetupEditorLib
  Utils
  DIL_Interface)

# installer options
add_custom_command(
  TARGET TestSuiteExecutorGUI
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:TestSuiteExecutorGUI> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
