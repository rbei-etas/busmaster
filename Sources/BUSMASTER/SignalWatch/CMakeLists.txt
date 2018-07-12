set(sources
  ../Application/MultiLanguage.cpp
  CheckBoxTree.cpp
  SignalWatch.cpp
  SignalWatch_CAN.cpp
  SignalWatch_J1939.cpp
  SignalWatchListBox.cpp
  MsgSignalSelect.cpp
  SignalWatch_LIN.cpp
  SigWatchAddDelDlg.cpp
  SigWatchDlg.cpp)

set(headers
  BaseSignalWatch_CAN.h
  BaseSignalWatch_J1939.h
  ../Application/MultiLanguage.h
  CheckBoxTree.h
  SignalWatch.h
  SignalWatch_CAN.h
  SignalWatch_Extern.h
  SignalWatch_J1939.h
  MsgSignalSelect.h
  SignalWatch_LIN.h
  SignalWatch_Resource.h
  SignalWatch_stdafx.h
  SignalWatchDefs.h
  SignalWatchListBox.h
  SigWatchAddDelDlg.h
  SigWatchDlg.h)

set(resources
  SignalWatch.rc)

add_library(SignalWatch SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(SignalWatch PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(SignalWatch
  DataTypes
  Utils
  DIL_Interface)

# installer options
add_custom_command(
  TARGET SignalWatch
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:SignalWatch> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
