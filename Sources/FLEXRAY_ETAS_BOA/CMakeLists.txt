set(sources
  ../DIL_Interface/ClientBuffer.cpp
  ../DIL_Interface/ClientList.cpp
  ../DIL_Interface/CommanDIL_Flexray.cpp
  FLEXRAY_ETAS_BOA.cpp
  FLEXRAY_ETAS_BOA_stdafx.cpp
  HardwareListing.cpp)

set(headers
  ../DIL_Interface/ClientBuffer.h
  ../DIL_Interface/ClientList.h
  ../DIL_Interface/CommanDIL_Flexray.h
  FLEXRAY_ETAS_BOA.h
  FLEXRAY_ETAS_BOA_Defs.h
  FLEXRAY_ETAS_BOA_Extern.h
  FLEXRAY_ETAS_BOA_Resource.h
  FLEXRAY_ETAS_BOA_stdafx.h
  HardwareListing.h)

set(resources
  FLEXRAY_ETAS_BOA.rc)

add_library(FLEXRAY_ETAS_BOA_1_4 SHARED ${sources} ${headers} ${resources})
add_library(FLEXRAY_ETAS_BOA_1_5 SHARED ${sources} ${headers} ${resources})
add_library(FLEXRAY_ETAS_BOA_2 SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)
target_compile_definitions(FLEXRAY_ETAS_BOA_1_4 PUBLIC -DBOA_VERSION=BOA_VERSION_1_4)
target_compile_definitions(FLEXRAY_ETAS_BOA_1_5 PUBLIC -DBOA_VERSION=BOA_VERSION_1_5)
target_compile_definitions(FLEXRAY_ETAS_BOA_2   PUBLIC -DBOA_VERSION=BOA_VERSION_2)

# linker options
set_target_properties(FLEXRAY_ETAS_BOA_1_4 PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
set_target_properties(FLEXRAY_ETAS_BOA_1_5 PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
set_target_properties(FLEXRAY_ETAS_BOA_2   PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(FLEXRAY_ETAS_BOA_1_4
  Advapi32
  DataTypes
  Utils)
target_link_libraries(FLEXRAY_ETAS_BOA_1_5
  Advapi32
  DataTypes
  Utils)
target_link_libraries(FLEXRAY_ETAS_BOA_2
  Advapi32
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET FLEXRAY_ETAS_BOA_1_4
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:FLEXRAY_ETAS_BOA_1_4> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
add_custom_command(
  TARGET FLEXRAY_ETAS_BOA_1_5
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:FLEXRAY_ETAS_BOA_1_5> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
add_custom_command(
  TARGET FLEXRAY_ETAS_BOA_2
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:FLEXRAY_ETAS_BOA_2> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
