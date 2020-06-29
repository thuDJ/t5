cmake_minimum_required(VERSION 3.10)
PROJECT(LIB_USE_EIGEN CXX C)

# Setup output directories
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# Set options
option(profile  "Compile with profiling flags"                   ON)
if(profile)
  list(APPEND cxxflags -pg -g -fno-omit-frame-pointer)
endif()

list(APPEND cxxflags -O2)

# submodule
#===============================================================================
# git SHA1 hash
#===============================================================================
find_package(Git)
if (GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
  execute_process(COMMAND git rev-parse HEAD
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
          RESULT_VARIABLE GIT_SHA1_SUCCESS
          OUTPUT_VARIABLE GIT_SHA1
          ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  if (GIT_SHA1_SUCCESS EQUAL 0)
    add_definitions(-DGIT_SHA1="${GIT_SHA1}")  # 为什么要add_definitions
    message(STATUS "GIT SHA: ${GIT_SHA1}")
  endif ()
endif ()

#===============================================================================
# Sync the config of submodules
#===============================================================================
if (GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
  execute_process(COMMAND git submodule sync
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMODULE_SYNC_SUCCESS)
  if (GIT_SUBMODULE_SYNC_SUCCESS EQUAL 0)
    message(STATUS "Synchronizing git submodules succeeded.")
  else()
    message(FATAL_ERROR "Synchronizing git submodules failed.")
  endif ()
endif ()



# 
INCLUDE_DIRECTORIES(/home/shenpf/workspace/MyCmake/t5/MyEigen1)
ADD_EXECUTABLE(libuseeigen src/main.cpp)
target_compile_options(libuseeigen PRIVATE ${cxxflags})





