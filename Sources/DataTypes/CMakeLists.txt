set(sources
  Cluster.cpp
  Defines_FlexRay.cpp
  FEALData.cpp
  Filter_Datatypes.cpp
  FLEXRAY_Datatypes.cpp
  J1939_DataTypes.cpp
  Log_Datatypes.cpp
  MainSubEntry.cpp
  MsgBufVSE.cpp
  MsgBufVVSE.cpp
  MsgSignal_Datatypes.cpp
  ProjConfig_DataTypes.cpp
  Struct_BUS.cpp
  UDS_DataTypes.cpp)

set(headers
  Base_FlexRay_Buffer.h
  Base_WrapperErrorLogger.h
  BaseAppServices.h
  BaseMsgBufAll.h
  Cluster.h
  DataTypes_stdafx.h
  DIL_Datatypes.h
  FEALData.h
  Filter_Datatypes.h
  FLEXRAY_Datatypes.h
  J1939_DataTypes.h
  Log_Datatypes.h
  MainSubEntry.h
  MsgBufAll_DataTypes.h
  MsgBufCANVFSE.h
  MsgBufFSE.h
  MsgBufLINVFSE.h
  MsgBufVFSE.h
  MsgBufVSE.h
  MsgBufVVSE.h
  MsgSignal_Datatypes.h
  ProjConfig_DataTypes.h
  SigGrphWnd_Datatypes.h
  SigWatch_Datatypes.h
  Typecode_FBX_entities.h
  UDS_DataTypes.h)

add_library(DataTypes STATIC ${sources} ${headers})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_LIB -Od)
remove_definitions(-GR -O2 -Ob2)

# linker options
target_link_libraries(DataTypes
  ${GETTEXT_LIBRARY})
