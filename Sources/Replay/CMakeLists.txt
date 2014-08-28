set(sources
  MsgReplayWnd.cpp
  ../Application/MultiLanguage.cpp
  OwnerDataListCtrl.cpp
  Replay.cpp
  ReplayFile.cpp
  ReplayFileConfigDlg.cpp
  ReplayManager.cpp
  ReplayProcess.cpp
  Utility_Replay.cpp)

set(headers
  MsgReplayWnd.h
  ../Application/MultiLanguage.h
  OwnerDataListCtrl.h
  Replay_Extern.h
  Replay_resource.h
  Replay_stdafx.h
  Replay_Struct.h
  ReplayFile.h
  ReplayFileConfigDlg.h
  ReplayManager.h
  ReplayProcess.h
  Utility_Replay.h)

set(resources
  Replay.rc)

add_library(Replay SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_AFXEXT)

# linker options
set_target_properties(Replay PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /STACK:8000000")
target_link_libraries(Replay
  DataTypes
  Utils
  DIL_Interface
  Filter
  Winmm)

# installer options
add_custom_command(
  TARGET Replay
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:Replay> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
