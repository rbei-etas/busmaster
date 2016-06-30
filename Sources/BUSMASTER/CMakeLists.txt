cmake_minimum_required(VERSION 2.8)
project(BUSMASTER)

#
# BUSMASTER version
#
set(BUSMASTER_VER 2.5.0)
add_definitions(-DBUSMASTER_VER=\"${BUSMASTER_VER}\")
set(CMAKE_BUILD_TYPE Release)

#
# required tools
#
include(FindPackageHandleStandardArgs)

# bison
find_path(BISON_PATH
  bison.exe
  PATHS ../Tools/bison)
find_program(BISON_EXECUTABLE
  NAMES bison.exe
  PATHS ${BISON_PATH})
find_package_handle_standard_args(BISON
  REQUIRED_VARS BISON_EXECUTABLE)

# flex
find_path(FLEX_PATH
  flex.exe
  PATHS ../Tools/flex)
find_program(FLEX_EXECUTABLE
  NAMES flex.exe
  PATHS ${FLEX_PATH})
find_package_handle_standard_args(FLEX
  REQUIRED_VARS FLEX_EXECUTABLE)

# gettext
find_path(GETTEXT_PATH
  lib/intl.lib
  PATHS EXTERNAL/GetText)
find_library(GETTEXT_LIBRARY
  NAMES intl
  PATHS ${GETTEXT_PATH}/lib)
find_path(GETTEXT_INCLUDE_DIR
  NAMES libintl.h
  PATHS ${GETTEXT_PATH}/include)
find_package_handle_standard_args(GETTEXT
  REQUIRED_VARS GETTEXT_LIBRARY GETTEXT_INCLUDE_DIR)

# iconv
find_path(ICONV_PATH
  lib/iconv.lib
  PATHS EXTERNAL/iconv)
find_library(ICONV_LIBRARY
  NAMES iconv
  PATHS ${ICONV_PATH}/lib)
find_path(ICONV_INCLUDE_DIR
  NAMES iconv.h
  PATHS ${ICONV_PATH}/include)
find_package_handle_standard_args(ICONV
  REQUIRED_VARS ICONV_LIBRARY ICONV_INCLUDE_DIR)

# libxml2
find_path(LIBXML2_PATH
  lib/libxml2.lib
  PATHS EXTERNAL/libxml2)
find_library(LIBXML2_LIBRARY
  NAMES libxml2
  PATHS ${LIBXML2_PATH}/lib)
find_path(LIBXML2_INCLUDE_DIR
  NAMES libxml/xmlversion.h
  PATHS ${LIBXML2_PATH}/include)
find_package_handle_standard_args(LIBXML2
  REQUIRED_VARS LIBXML2_LIBRARY LIBXML2_INCLUDE_DIR)

# atl
find_path(ATL_PATH
  lib/ATL/i386/atl.lib
  PATHS C:/WinDDK/7600.16385.1)
set(ATL_INCLUDE_DIRS
  ${ATL_PATH}/inc/atl71)
set(ATL_LIBRARIES
  ${ATL_PATH}/lib/ATL/i386/atl.lib
  ${ATL_PATH}/lib/ATL/i386/atls.lib
  ${ATL_PATH}/lib/ATL/i386/atlthunk.lib)

# mfc
find_path(MFC_PATH
  lib/Mfc/i386/mfc42.lib
  PATHS C:/WinDDK/7600.16385.1)
set(MFC_INCLUDE_DIRS
  ${MFC_PATH}/inc/mfc42)
set(MFC_LIBRARIES
  ${MFC_PATH}/lib/Mfc/i386/mfc42.lib
  ${MFC_PATH}/lib/Mfc/i386/mfcs42.lib)

#
# common compiler/linker settings
#
add_definitions(-DWINVER=0x0500) # Windows 2000
#add_definitions(-DWINVER=0x0501) # Windows XP
#add_definitions(-DWINVER=0x0502) # Windows Server 2003
#add_definitions(-DWINVER=0x0600) # Windows Vista
#add_definitions(-DWINVER=0x0601) # Windows 7
#add_definitions(-D_VC80_UPGRADE=0x0710)
link_directories(
  ${ATL_PATH}/lib/ATL/i386
  ${MFC_PATH}/lib/Mfc/i386)
set(CMAKE_MFC_FLAG 2) # shared MFC library
add_definitions(-D_CRT_SECURE_NO_WARNINGS -wd4996) # warning C4996: '...': This function or variable may be unsafe. Consider using ... instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.

#
# sub projects
#
add_subdirectory(Application)
add_subdirectory(BusEmulation)
add_subdirectory(CAN_ETAS_BOA)
add_subdirectory(CAN_ICS_neoVI)
add_subdirectory(CAN_ISOLAR_EVE_VCAN)
add_subdirectory(CAN_iView)
add_subdirectory(CAN_IXXAT_VCI)
add_subdirectory(CAN_Kvaser_CAN)
add_subdirectory(CAN_MHS)
add_subdirectory(CAN_NSI)
add_subdirectory(CAN_PEAK_USB)
add_subdirectory(CAN_STUB)
add_subdirectory(CAN_Vector_XL)
add_subdirectory(CAN_VSCOM)
add_subdirectory(CommonClass)
add_subdirectory(DataTypes)
add_subdirectory(DIL_Interface)
add_subdirectory(DIL_J1939)
add_subdirectory(EXTERNAL)
add_subdirectory(Filter)
add_subdirectory(FLEXRAY_ETAS_BOA)
# add_subdirectory(Format Converter)
add_subdirectory(FrameProcessor)
add_subdirectory(LIN_ETAS_BOA)
add_subdirectory(LIN_ISOLAR_EVE_VLIN)
add_subdirectory(LIN_PEAK_USB)
add_subdirectory(LIN_Vector_XL)
# add_subdirectory(Localization)
add_subdirectory(MHS_CNFG)
add_subdirectory(NodeSimEx)
add_subdirectory(ProjectConfiguration)
add_subdirectory(PSDI_CAN)
add_subdirectory(Replay)
add_subdirectory(SigGrphWnd)
add_subdirectory(SignalDefiner)
add_subdirectory(SignalWatch)
add_subdirectory(TestSetupEditorGUI)
add_subdirectory(TestSetupEditorLib)
add_subdirectory(TestSuiteExecutorGUI)
add_subdirectory(TXWindow)
add_subdirectory(TXWindowFlexRay)
add_subdirectory(UDS_Protocol)
add_subdirectory(Utility)
