cmake_minimum_required(VERSION 2.8)

project(ocl_demo)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions( -Wall -Wl,--no-as-needed -pthread -lpthread -std=gnu++11 -DGL_GLEXT_PROTOTYPES )
elseif (MSVC)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package( OpenCL REQUIRED )

if(UNIX)
	include_directories( ${_OPENCL_INCLUDE_DIR} )
endif()
if(WIN32)
	include_directories( ${_OPENCL_CPP_INCLUDE_DIRS} )
endif()

foreach ( _variableName ${OpenCL_INCLUDE_DIR} )
    message(STATUS ${_variableName} )
endforeach()

add_executable( ocl_tut1
                ./main_demo.cpp )
                
foreach ( _variableName ${OPENCL_LIBRARIES} )
    message(STATUS ${_variableName} )
endforeach()
                
configure_file( ./forTest.cl ${CMAKE_CURRENT_BINARY_DIR}/forTest.cl COPYONLY )
                
target_link_libraries( ocl_tut1 ${OPENCL_LIBRARIES} )
