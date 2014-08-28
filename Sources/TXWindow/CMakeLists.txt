set(sources
  CMsgSignalTemp.cpp
  FlexListCtrl.cpp
  ../Application/MultiLanguage.cpp
  SignalDetailsDlg.cpp
  Tx_MDIChildBase.cpp
  TxFlags.cpp
  TxFunctionsView.cpp
  TxMsgBlocksView.cpp
  TxMsgChildFrame.cpp
  TxMsgDetailsView.cpp
  TxMsgListView.cpp
  TxMsgManager.cpp
  TXWindow.cpp
  TxWndDataStore.cpp)

set(headers
  CMsgSignalTemp.h
  FlexListCtrl.h
  ../Application/MultiLanguage.h
  SignalDetailsDlg.h
  Tx_MDIChildBase.h
  TxFlags.h
  TxFunctionsView.h
  TxMsgBlocksView.h
  TxMsgChildFrame.h
  TxMsgDetailsView.h
  TxMsgListView.h
  TxMsgManager.h
  TxWindow_resource.h
  TxWindow_stdafx.h
  TXWnd_Defines.h
  TxWnd_Extern.h
  TxWndDataStore.h)

set(resources
  TXWindow.rc)

add_library(TXWindow SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_AFXEXT)

# linker options
set_target_properties(TXWindow PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /STACK:8000000")
target_link_libraries(TXWindow
  Winmm
  DataTypes
  Utils
  DIL_Interface)

# installer options
add_custom_command(
  TARGET TXWindow
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:TXWindow> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
