set(sources
  ClientBuffer.cpp
  ClientList.cpp
  CommanDIL_Flexray.cpp
  CommanDIL_LIN.cpp
  DILL_Dummy.cpp
  DIL_CAN.cpp
  DIL_FLEXRAY.cpp
  DIL_Interface.cpp
  DIL_Interface_stdafx.cpp
  DILC_Dummy.cpp
  DILI_J1939.cpp
  DIL_LIN.cpp)

set(headers
  BaseDIL_CAN.h
  BaseDIL_CAN_Controller.h
  BaseDIL_FLEXRAY.h
  BaseDIL_FLEXRAY_Controller.h
  BaseDIL_J1939.h
  ClientBuffer.h
  ClientList.h
  CommanDIL_Flexray.h
  CommanDIL_LIN.h
  DILL_Dummy.h
  DIL_CAN.h
  DIL_FLEXRAY.h
  DIL_Interface.h
  DIL_Interface_extern.h
  DIL_Interface_stdafx.h
  DILC_Dummy.h
  DILI_J1939.h
  DIL_LIN.h)

add_library(DIL_Interface SHARED ${sources} ${headers})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL -Od)

# linker options
set_target_properties(DIL_Interface PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(DIL_Interface
  CAN_ICS_neoVI
  CAN_Kvaser_CAN
  CAN_MHS
  CAN_STUB
  CAN_Vector_XL
  CAN_VSCOM
  DIL_J1939
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET DIL_Interface
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:DIL_Interface> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
