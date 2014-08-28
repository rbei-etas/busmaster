set(sources
  ../DIL_Interface/CommanDIL_LIN.cpp
  HardwareListing.cpp
  LIN_PEAK_USB.cpp
  LIN_PEAK_USB_Channel.cpp
  LIN_PEAK_USB_Network.cpp
  LIN_PEAK_USB_stdafx.cpp)

set(headers
  ../DIL_Interface/CommanDIL_LIN.h
  HardwareListing.h
  EXTERNAL/PLinApi.h
  LIN_PEAK_USB.h
  LIN_PEAK_USB_Channel.h
  LIN_PEAK_USB_Extern.h
  LIN_PEAK_USB_Network.h
  LIN_PEAK_USB_Resource.h
  LIN_PEAK_USB_stdafx.h)

set(resources
  LIN_PEAK_USB.rc)

add_library(LIN_PEAK_USB SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(LIN_PEAK_USB PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid /STACK:8000000")
target_link_libraries(LIN_PEAK_USB
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET LIN_PEAK_USB
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:LIN_PEAK_USB> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
