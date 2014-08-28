set(sources
  AcceptanceFilterDlg.cpp
  CAN_NSI.cpp
  CAN_NSI_stdafx.cpp
  CANRegAnalysis.cpp
  ChangeRegisters.cpp
  HardwareListing.cpp
  ../Application/MultiLanguage.cpp)

set(headers
  AcceptanceFilterDlg.h
  CAN_NSI.h
  CAN_NSI_Extern.h
  CAN_NSI_Resource.h
  CAN_NSI_stdafx.h
  ChangeRegDefines.h
  ChangeRegisters.h
  HardwareListing.h
  ../Application/MultiLanguage.h)

set(resources
  CAN_NSI.rc)

add_library(CAN_NSI SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_NSI PROPERTIES LINK_FLAGS "/STACK:8000000 /SAFESEH:NO")
target_link_libraries(CAN_NSI
  ${CMAKE_CURRENT_SOURCE_DIR}/NSICANEX.lib
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET CAN_NSI
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_NSI> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
