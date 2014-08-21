set(sources
  ExportLogFileDlg.cpp
  LogToExcel.cpp
  LogToExcelConverter.cpp
  ../FormatConverterApp/MultiLanguage.cpp
  ../../Utility/MultiLanguageSupport.cpp)

set(headers
  Defines.h
  ExportLogFileDlg.h
  LogToExcel.h
  LogToExcelConverter.h
  Resource.h
  ../FormatConverterApp/MultiLanguage.h
  ../../Utility/MultiLanguageSupport.h)

set(resources
  LogToExcelConverter.rc)

add_library(LogToExcelConverter SHARED ${sources} ${headers} ${resources})

add_definitions(-D_AFXEXT)

include_directories(
  ${GETTEXT_INCLUDE_DIR}
  ${MFC_INCLUDE_DIRS})

target_link_libraries(LogToExcelConverter
  ${GETTEXT_LIBRARY}
  ${MFC_LIBRARIES})

add_custom_command(
  TARGET LogToExcelConverter
  POST_BUILD
  COMMAND cmake ARGS -E make_directory ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/
  COMMAND cmake ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/LogToExcelConverter.dll ${PROJECT_SOURCE_DIR}/../BIN/${CMAKE_BUILD_TYPE}/ConverterPlugins/)
