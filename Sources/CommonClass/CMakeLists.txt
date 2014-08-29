set(sources
  GUI_FormatMsgCAN.cpp
  GUI_FormatMsgCommon.cpp
  GUI_FormatMsgFlexRay.cpp
  GUI_FormatMsgJ1939.cpp
  GUI_FormatMsgLIN.cpp
  MsgContainerBase.cpp
  RefTimeKeeper.cpp)

set(headers
  CommonClass_stdafx.h
  GUI_FormatMsgCAN.h
  GUI_FormatMsgCommon.h
  GUI_FormatMsgFlexRay.h
  GUI_FormatMsgJ1939.h
  GUI_FormatMsgLIN.h
  MsgContainerBase.h
  RefTimeKeeper.h)

add_library(CommonClass STATIC ${sources} ${headers})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_LIB)

# linker options
target_link_libraries(CommonClass
  Utils)
