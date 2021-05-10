project(pomcp-1.0)
cmake_minimum_required(VERSION 3.0)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

set(SOURCE_FILES
src/battleship.cpp
src/beliefstate.cpp
src/cellar.cpp
src/coord.cpp
src/experiment.cpp
src/main.cpp
src/mcts.cpp
src/network.cpp
src/node.cpp
src/pocman.cpp
src/rocksample.cpp
src/simulator.cpp
src/tag.cpp
src/testsimulator.cpp
src/utils.cpp
)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(pomcp ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( pomcp LINK_PUBLIC ${BOOST_LIBRARIES} boost_program_options)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

install(TARGETS pomcp
  ARCHIVE DESTINATION ${dir}/lib
  LIBRARY DESTINATION ${dir}/lib
  RUNTIME DESTINATION ${dir}/bin
)
