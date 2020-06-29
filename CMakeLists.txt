cmake_minimum_required(VERSION 3.10)
PROJECT(LIB_USE_EIGEN CXX C)

# Setup output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# Set options
option(profile  "Compile with profiling flags"                   OFF)
if(profile)
  list(APPEND cxxflags -pg -g -fno-omit-frame-pointer)
  list(APPEND exelinkflags -pg)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

list(APPEND cxxflags -O3)

# MPI
set(MPI_USE OFF)
if (DEFINED mpi)                           
  if (mpi)
    find_package(MPI REQUIRED) 
    set(MPI_USE ON)
  endif ()
else ()
  find_package(MPI)
  if (MPI_CXX_FOUND)
    set(MPI_USE ON)
  endif ()
endif ()

message(STATUS "MPI parallel: turned ${MPI_USE}")
if (MPI_USE)
  add_definitions(-DUSE_MPI)                        
  list(APPEND libraries ${MPI_CXX_LIBRARIES})         
  include_directories(${MPI_CXX_INCLUDE_PATH})
  message(STATUS "MPI compiler: ${MPI_CXX_COMPILER}")
endif ()

# xtensor
if (NOT (CMAKE_VERSION VERSION_LESS 3.13))
  cmake_policy(SET CMP0079 NEW)
endif()

add_subdirectory(dependences/xtl)
add_subdirectory(dependences/xtensor)
target_link_libraries(xtensor INTERFACE xtl)
list(APPEND libraries xtensor)      


# submodule
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

# include
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/dependences/Eigen)
ADD_EXECUTABLE(libuseeigen src/main.cpp)
target_compile_options(libuseeigen PRIVATE ${cxxflags})
target_link_libraries(libuseeigen ${libraries})

# install
set(install_dir ${CMAKE_SOURCE_DIR}/install)
install(TARGETS libuseeigen
  RUNTIME DESTINATION ${install_dir}/bin
  LIBRARY DESTINATION ${install_dir}/lib
  ARCHIVE DESTINATION ${install_dir}/lib
)


