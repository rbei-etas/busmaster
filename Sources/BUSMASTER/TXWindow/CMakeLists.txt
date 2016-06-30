set(sources
  CCheckColumnTreeCtrl.cpp
  ConfigDetails.cpp
  FlexListCtrl.cpp
  HeaderCtrlEx.cpp
  CHexEdit/HexEdit.cpp
  LinFlexListCtrl.cpp
  LINScheduleDataStore.cpp
  ScheduleTableCfgDlg.cpp
  TransmitMsg.cpp
  TransmitMsg_LIN.cpp
  TxLINDataStore.cpp
  Tx_MDIChildBase.cpp
  TxFlagsFlexRay.cpp
  TxFlexRayDataStore.cpp
  TxFlexrayMsgHandlerDlg.cpp
  TxListComboBox.cpp
  TxMsgChildFrame.cpp
  TxMsgList.cpp
  TXWindowFlexRay.cpp
  TxWindowFlexRay_stdafx.cpp)

set(headers
  CCheckColumnTreeCtrl.h
  ConfigDetails.h
  FlexListCtrl.h
  HashDefines.h
  HeaderCtrlEx.h
  CHexEdit/HexEdit.h
  LinFlexListCtrl.h
  LINScheduleDataStore.h
  ScheduleTableCfgDlg.h
  TransmitMsg.h
  TransmitMsg_LIN.h
  TxLINDataStore.h
  Tx_MDIChildBase.h
  TxFlagsFlexRay.h
  TxFlexRayDataStore.h
  TxFlexrayMsgHandlerDlg.h
  TxListComboBox.h
  TxMsgChildFrame.h
  TxMsgList.h
  TxWindowFlexRay_resource.h
  TxWindowFlexRay_stdafx.h
  TXWndFlexRay_Defines.h
  TxWndFlexRay_Extern.h)

set(resources
  TXWindowFlexRay.rc)

add_library(TXWindowFlexRay SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_AFXEXT)

# linker options
target_link_libraries(TXWindowFlexRay
  Winmm
  DIL_Interface
  Utils
  DataTypes)

# installer options
add_custom_command(
  TARGET TXWindowFlexRay
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:TXWindowFlexRay> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
