cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

project(MeshIO)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if( ITK_VERSION_MAJOR MATCHES "3" )
  if( NOT ITK_USE_REVIEW )
    message(FATAL_ERROR "ITK must be build with ITK_USE_REVIEW turned ON")
  endif( NOT ITK_USE_REVIEW )
endif()

set(EXECUTABLE_OUTPUT_PATH ${MeshIO_BINARY_DIR}/bin CACHE INTERNAL "Where to put the executables")
set(LIBRARY_OUTPUT_PATH ${MeshIO_BINARY_DIR}/bin CACHE INTERNAL "Where to put the libraries")


include(CTest)

#INCLUDE_DIRECTORIES(
#  ${PROJECT_SOURCE_DIR}/Source
#  )

# Disable MSVC 8 warnings
if(WIN32)
  option(DISABLE_MSVC8_DEPRECATED_WARNINGS
         "Disable Visual Studio 8 deprecated warnings" ON)
  mark_as_advanced(FORCE DISABLE_MSVC8_DEPRECATED_WARNINGS)
  if(DISABLE_MSVC8_DEPRECATED_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  endif(DISABLE_MSVC8_DEPRECATED_WARNINGS)
endif(WIN32)

set(GIFTI_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Utilities/gifticlib)
include_directories(BEFORE ${ITK_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/MeshIO
  ${GIFTI_INCLUDE_DIR}
)

add_subdirectory( Utilities )
add_subdirectory( MeshIO )
add_subdirectory( Testing )
add_subdirectory( Examples )

