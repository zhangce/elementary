cmake_minimum_required (VERSION 2.6)

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

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -p")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -p")

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -p")

#endif()


set(Boost_NO_SYSTEM_PATHS "ON") 
set(Boost_NO_BOOST_CMAKE "ON")

find_package (Boost COMPONENTS program_options)

if(NOT Boost_FOUND)
message(STATUS "Set BOOST_ROOT to ${CMAKE_CURRENT_SOURCE_DIR}/dep/boost")
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/dep/boost")
endif()

find_package (Boost COMPONENTS program_options REQUIRED)

message(STATUS "Boost Headers Path: " ${Boost_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})

message(STATUS "Boost Library Path:"${Boost_PROGRAM_OPTIONS_LIBRARY})

target_link_libraries (bin/elementary ${Boost_PROGRAM_OPTIONS_LIBRARY})

target_link_libraries (bin/elementary -lrt)
