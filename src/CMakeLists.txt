project(pomcp-1.0)
cmake_minimum_required(VERSION 3.0)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

set(SOURCE_FILES
battleship.cpp
beliefstate.cpp
cellar.cpp
coord.cpp
experiment.cpp
main.cpp
mcts.cpp
network.cpp
node.cpp
pocman.cpp
rocksample.cpp
simulator.cpp
tag.cpp
testsimulator.cpp
utils.cpp
)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(pomcp ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( pomcp LINK_PUBLIC ${BOOST_LIBRARIES} boost_program_options)

set(LIB_DESTINATION "/lib")
set(BIN_DESTINATION "/bin")

install(TARGETS pomcp
  ARCHIVE DESTINATION ${LIB_DESTINATION}
  LIBRARY DESTINATION ${LIB_DESTINATION}
  RUNTIME DESTINATION ${BIN_DESTINATION}
)

