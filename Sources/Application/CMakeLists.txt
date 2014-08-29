set(sources
  Application.cpp
  AppServices_Impl.cpp
  BUSMASTER.cpp
  BusmasterDump.cpp
  BusStatisticCAN.cpp
  BusStatisticLIN.cpp
  BusStatistics.cpp
  BusStatisticsDlg.cpp
  CGCtrl.cpp
  CNVTCImageList.cpp
  CNVTCToolBar.cpp
  ConfigAdapter.cpp
  ConfigData.cpp
  ConfigDetails.cpp
  ConfigMsgDispPropPg.cpp
  ConfigMsgLogDlg.cpp
  DatabaseDissociateDlg.cpp
  DisplayIntMsg.cpp
  DriverInfoDlg.cpp
  ElementSelection.cpp
  FibexClass.cpp
  ChannelConfigurationDlg.cpp
  FIBEX_Structs.cpp
  Flags.cpp
  FlexRayDatabaseDissociateDlg.cpp
  FlexRayNetworkStats.cpp
  FlexRayNetworkStatsDlg.cpp
  FlexTxHandler.cpp
  GraphConfiguration.cpp
  GraphElement.cpp
  GraphList.cpp
  GraphParameters.cpp
  InterfaceGetter.cpp
  J1939TimeOutCfg.cpp
  LDFLexer.c
  LDFParser.cpp
  LINBusStatisticsDlg.cpp
  MainFrm.cpp
  MDIChildBase.cpp
  MessageAttrib.cpp
  MessageDetailsDlg.cpp
  MessageInterpretation.cpp
  MessageList.cpp
  MsgBufferConfigPage.cpp
  MsgFilterConfigPage.cpp
  MsgFrmtWnd.cpp
  MsgIDAttr.cpp
  MsgSgDetView.cpp
  MsgSgTreeView.cpp
  MsgSignal.cpp
  MsgSignalDBWnd.cpp
  MsgWndHdrInfo.cpp
  MsgWndThread.cpp
  MultiLanguage.cpp
  NetworkStatistics.cpp
  NodeUtil_Flexray.cpp
  NotificListbox.cpp
  NotificWnd.cpp
  ParserHelper.cpp
  PPageMessage.cpp
  Properties.cpp
  RxMsgList.cpp
  SigGrphConfigDlg.cpp
  SigGrphWndHandler.cpp
  SignalDetailsDlg.cpp
  SignalMatrix.cpp
  Splash.cpp
  SplFileDlg.cpp
  StdAfx.cpp
  Struct.cpp
  TimeManager.cpp
  TreeItemParam.cpp
  TSEditorHandler.cpp
  TSExecutorHandler.cpp
  TxHandler.cpp
  TxHandlerLIN.cpp
  TxMsgWndJ1939.cpp
  UIThread.cpp
  Utility.cpp
  ValueDescriptionDlg.cpp
  WaveFormDataHandler.cpp
  WaveformSelectionDlg.cpp
  WaveformTransmitter.cpp
  WrapperErrorLogger.cpp)

set(headers
  AppServices_Impl.h
  BaseBusStatisticCAN.h
  BaseBusStatisticLIN.h
  BUSMASTER.h
  BusmasterDump.h
  BusStatisticCAN.h
  BusStatisticLIN.h
  BusStatistics.h
  BusStatisticsDlg.h
  CFilesDefs_CAN.h
  CGCtrl.h
  CNVTCImageList.h
  CNVTCToolBar.h
  common.h
  ConfigAdapter.h
  ConfigData.h
  ConfigDetails.h
  ConfigMsgLogDlg.h
  CSignalGeneration.h
  DatabaseDissociateDlg.h
  DataType.h
  DisplayIntMsg.h
  DriverInfoDlg.h
  ElementSelection.h
  FibexClass.h
  FibexClass_extern.h
  ChannelConfigurationDlg.h
  Fibex_Defs.h
  FIBEX_Structs.h
  Flags.h
  FlexRayDatabaseDissociateDlg.h
  FlexRayNetworkStats.h
  FlexRayNetworkStatsDlg.h
  FlexTxHandler.h
  GraphConfiguration.h
  GraphElement.h
  GraphList.h
  GraphParameters.h
  HashDefines.h
  InterfaceGetter.h
  J1939TimeOutCfg.h
  LDFParser.h
  LINBusStatisticsDlg.h
  MainFrm.h
  MDIChildBase.h
  MessageAttrib.h
  MessageDetailsDlg.h
  MessageInterpretation.h
  MessageList.h
  MsgBufferConfigPage.h
  MsgFilterConfigPage.h
  MsgFrmtWnd.h
  MsgIDAttr.h
  MsgSgDetView.h
  MsgSgTreeView.h
  MsgSignal.h
  MsgSignalDBWnd.h
  MsgWndHdrInfo.h
  MsgWndThread.h
  MultiLanguage.h
  NetworkStatistics.h
  NodeUtil_Flexray.h
  NotificListbox.h
  NotificWnd.h
  ParserHelper.h
  PPageMessage.h
  Properties.h
  PSDI_Extern.h
  Resource.h
  RxMsgList.h
  SectionNames.h
  SigGrphConfigDlg.h
  SigGrphWndHandler.h
  SignalDetailsDlg.h
  SignalMatrix.h
  Splash.h
  SplFileDlg.h
  StdAfx.h
  Struct.h
  TimeManager.h
  TreeItemParam.h
  TSEditorHandler.h
  TSExecutorHandler.h
  TxHandler.h
  TxHandlerLIN.h
  TxMsgWndJ1939.h
  UIThread.h
  Utility.h
  ValueDescriptionDlg.h
  WaveFormDataHandler.h
  WaveformSelectionDlg.h
  WaveformTransmitter.h
  WrapperErrorLogger.h)

set(resources
  BUSMASTER.rc
  CAN_MonitorApp.idl)

add_executable(BUSMASTER WIN32 ${sources} ${headers} ${resources})

add_custom_command(
  OUTPUT LDFLexer.c
  COMMAND ${FLEX_EXECUTABLE}
  ARGS -i -L -oLDFLexer.c ${CMAKE_CURRENT_SOURCE_DIR}/LDFLexer.l
  DEPENDS LDFLexer.l)

add_custom_command(
  OUTPUT LDFParser.cpp LDFParser.h
  COMMAND ${BISON_EXECUTABLE}
  ARGS -d -l -o${CMAKE_CURRENT_BINARY_DIR}/LDFParser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/LDFParser.y
  DEPENDS LDFParser.y
  WORKING_DIRECTORY ${BISON_PATH})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/$(IntDir)
  ${CMAKE_CURRENT_BINARY_DIR}/../BusEmulation/BusEmulation.dir/${CMAKE_BUILD_TYPE}/ # workaround to find BusEmulation.h
  ..
  ${MFC_INCLUDE_DIRS}
  ${ATL_INCLUDE_DIRS}
  ${LIBXML2_INCLUDE_DIR}
  ${ICONV_INCLUDE_DIR}
  ../Include)

add_dependencies(BUSMASTER
  BusEmulation)

# compiler options
add_definitions(-Od -GF)

# linker options
set_target_properties(BUSMASTER PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(BUSMASTER
  Winmm shlwapi htmlhelp psapi
  CommonClass
  DataTypes
  Utils
  DIL_Interface
  Filter
  FrameProcessor
  NodeSimEx
  ProjectConfiguration
  PSDI_CAN
  Replay
  SigGrphWnd
  SignalDefiner
  SignalWatch
  TestSetupEditorGUI
  TestSuiteExecutorGUI
  TXWindow
  UDS_Protocol)

# installer options
add_custom_command(
  TARGET BUSMASTER
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:BUSMASTER> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/GCCDLLMakeTemplate_CAN ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/GCCDLLMakeTemplate_J1939 ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/GCCDLLMakeTemplate_LIN ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Res/Splsh16.bmp ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/BUSMASTER.exe.manifest ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/BUSMASTER.dir/${CMAKE_CFG_INTDIR}/CAN_MonitorApp.tlb ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/BUSMASTER.tlb
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/BUSMASTER.dir/${CMAKE_CFG_INTDIR}/CAN_MonitorApp.h ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/BUSMASTER_Interface.h
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/BUSMASTER.dir/${CMAKE_CFG_INTDIR}/CAN_MonitorApp_i.c ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/BUSMASTER_Interface.c)
