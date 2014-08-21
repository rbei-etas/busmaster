cmake_minimum_required(VERSION 2.8)
project(FormatConverter)

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
  PATHS ../../Tools/bison)
find_program(BISON_EXECUTABLE
  NAMES bison.exe
  PATHS ${BISON_PATH})
find_package_handle_standard_args(BISON
  REQUIRED_VARS BISON_EXECUTABLE)

# flex
find_path(FLEX_PATH
  flex.exe
  PATHS ../../Tools/flex)
find_program(FLEX_EXECUTABLE
  NAMES flex.exe
  PATHS ${FLEX_PATH})
find_package_handle_standard_args(FLEX
  REQUIRED_VARS FLEX_EXECUTABLE)

# gettext
find_path(GETTEXT_PATH
  lib/intl.lib
  PATHS ../EXTERNAL/GetText)
find_library(GETTEXT_LIBRARY
  NAMES intl
  PATHS ${GETTEXT_PATH}/lib)
find_path(GETTEXT_INCLUDE_DIR
  NAMES libintl.h
  PATHS ${GETTEXT_PATH}/include)
find_package_handle_standard_args(GETTEXT
  REQUIRED_VARS GETTEXT_LIBRARY GETTEXT_INCLUDE_DIR)

# mfc
find_path(MFC_PATH
  relnote.htm
  PATHS C:/WinDDK/7600.16385.1)
set(MFC_INCLUDE_DIRS
  ${MFC_PATH}/inc/mfc42
  ${MFC_PATH}/inc/atl71)
set(MFC_LIBRARIES
  ${MFC_PATH}/lib/Mfc/i386/mfc42.lib
  ${MFC_PATH}/lib/Mfc/i386/mfcs42.lib
  ${MFC_PATH}/lib/ATL/i386/atl.lib
  ${MFC_PATH}/lib/ATL/i386/atlthunk.lib)
set(CMAKE_MFC_FLAG 2) # shared MFC library

# zlib
find_path(ZLIB_PATH
  lib/zlib.lib
  PATHS ../EXTERNAL/zlib)
find_library(ZLIB_LIBRARY
  NAMES zlib
  PATHS ${ZLIB_PATH}/lib)
find_path(ZLIB_INCLUDE_DIR
  NAMES zlib.h
  PATHS ${ZLIB_PATH}/include)
find_package_handle_standard_args(ZLIB
  REQUIRED_VARS ZLIB_LIBRARY ZLIB_INCLUDE_DIR)

#
# sub projects
#
add_subdirectory(AscLogConverter)
add_subdirectory(BlfLibrary)
add_subdirectory(BlfLogConverter)
add_subdirectory(CAPL2CConverter)
add_subdirectory(DBC2DBFConverter)
add_subdirectory(DBC2DBFConverterLibrary)
add_subdirectory(DBF2DBCConverter)
add_subdirectory(FormatConverterApp)
add_subdirectory(J1939DBC2DBFConverter)
add_subdirectory(LogAscConverter)
add_subdirectory(LogToExcelConverter)
