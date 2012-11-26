cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_library (SMan1 src/SMan/gen-cpp/Hbase.cpp
src/SMan/gen-cpp/hbase_constants.cpp src/SMan/gen-cpp/hbase_types.cpp)

#### GCC OPTIMIZATION ####
IF(DEFINED CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of
   build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug
   Release RelWithDebInfo MinSizeRel.")
ELSE()
   SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build,
   options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release
   RelWithDebInfo MinSizeRel.")
   ENDIF()
   message(STATUS "Use build optimizations: " ${CMAKE_BUILD_TYPE})

#### Project Mia! ####

#IF(APPLE)
#message(STATUS "APPLE:" ${APPLE})
#ENDIF()
#
#message(STATUS "Platform: " ${CMAKE_SYSTEM})
#message(STATUS "Platform: " ${CMAKE_SYSTEM_NAME})

project (Mia)

add_executable( bin/elementary src/elly/elly/main.cpp )

message(STATUS "Build example for Mia::Elementary")
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/examples/elly/config_test_lda.cfg.in" "${CMAKE_CURRENT_SOURCE_DIR}/examples/elly/config_test_lda.cfg")
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/examples/elly/LDA/catelog.cfg.in"
"${CMAKE_CURRENT_SOURCE_DIR}/examples/elly/LDA/catelog.cfg")

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/examples/elly/config_test_mln.cfg.in" "${CMAKE_CURRENT_SOURCE_DIR}/examples/elly/config_test_mln.cfg")
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/examples/elly/MLN/catelog.cfg.in"
"${CMAKE_CURRENT_SOURCE_DIR}/examples/elly/MLN/catelog.cfg")


#### Find dependencies ####

## Boost ##

#if(OPENMP_FOUND)

# message(STATUS "Find OpenMP")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -fpermissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fpermissive")

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -p")

#endif()

#find_package (Java)
#find_package (Java_JVM)

find_package (JNI)

if(JNI_FOUND)
	message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
	message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
	message (STATUS "JAVA_JVM_LIBRARY=${JAVA_JVM_LIBRARY}")
else()
	message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
	message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
	message (STATUS "JAVA_JVM_LIBRARY=${JAVA_JVM_LIBRARY}")

	if(NOT JNI_LIBRARIES OR NOT JNI_INCLUDE_DIRS)
		message(FATAL_ERROR "JNI not found! Set JNI_LIBRARIES and JNI_INCLUDE_DIRS")	
	else()
		message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
		message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
	endif()
endif()

find_package(Thrift REQUIRED)

if(Thrift_FOUND)
	message (STATUS "Thrift_INCLUDE_DIR=${Thrift_INCLUDE_DIR}")
	message (STATUS "Thrift_LIB=${Thrift_LIB}")
else()
	if(NOT Thrift_INCLUDE_DIR OR NOT Thrift_LIB)
		message(FATAL_ERROR "Thrift not found! Set Thrift_INCLUDE_DIR and Thrift_LIB")	
	else()
		message (STATUS "Thrift_INCLUDE_DIR=${Thrift_INCLUDE_DIR}")
		message (STATUS "Thrift_LIB=${Thrift_LIB}")
	endif()
endif()

target_link_libraries (SMan1 "-L${Thrift_LIB} -lthrift")
target_link_libraries (bin/elementary "-L${Thrift_LIB} -lthrift")
include_directories("${Thrift_INCLUDE_DIR}/thrift")


set(Boost_NO_SYSTEM_PATHS "ON") 
set(Boost_NO_BOOST_CMAKE "ON")

find_package (Boost COMPONENTS program_options)

if(NOT Boost_FOUND)
message(STATUS "Set BOOST_ROOT to ${CMAKE_CURRENT_SOURCE_DIR}/dep/boost")
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/dep/boost")
endif()



if(NOT Boost_PROGRAM_OPTIONS_LIBRARY)
if(NOT Boost_INCLUDE_DIRS)
find_package (Boost COMPONENTS program_options REQUIRED)
endif()
endif()


message(STATUS "Boost Headers Path: " ${Boost_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${JNI_INCLUDE_DIRS})

message(STATUS "Boost Library Path:"${Boost_PROGRAM_OPTIONS_LIBRARY})

target_link_libraries (bin/elementary ${Boost_PROGRAM_OPTIONS_LIBRARY})

target_link_libraries (bin/elementary SMan1)



if(NOT APPLE)
target_link_libraries (bin/elementary -lrt)
target_link_libraries (bin/elementary ${JAVA_JVM_LIBRARY})
else()
target_link_libraries (bin/elementary ${JNI_LIBRARIES})
endif()





