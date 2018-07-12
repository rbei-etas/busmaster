set(sources
  BaseEntityTA.cpp
  DataBaseMsgList.cpp
  Send_MessageEntity.cpp
  SendEntity.cpp
  TestCaseEntity.cpp
  TestSetupEntity.cpp
  TestSetupHeader.cpp
  TSDefinitions.cpp
  Verify_MessageEntity.cpp
  VerifyEntity.cpp
  VerifyResponse.cpp)

set(headers
  BaseEntityTA.h
  DataBaseMsgList.h
  Send_MessageEntity.h
  SendEntity.h
  TestCaseEntity.h
  TestSetupEditorLib_stdafx.h
  TestSetupEntity.h
  TestSetupHeader.h
  TSDefinitions.h
  Verify_MessageEntity.h
  VerifyEntity.h
  VerifyResponse.h)

set(resources
  TestSetupEditorLib.rc)

add_library(TestSetupEditorLib STATIC ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_LIB)
