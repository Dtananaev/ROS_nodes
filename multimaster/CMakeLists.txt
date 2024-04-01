cmake_minimum_required(VERSION 2.8.3)
project(multimaster)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

MESSAGE("FLAGS: ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
  roslib
  std_msgs
  topic_tools
)

# # Declare a catkin package
catkin_package()

# # Build talker and listener
include_directories(include ${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})

add_executable(foreign2host src/foreign2host.cpp
                            src/multimaster.cpp
                            src/relay_topic.cpp
                            src/relay_config.cpp
                            src/relay_manager.cpp
                            src/relay_tf.cpp)
target_link_libraries(foreign2host ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_executable(host2foreign src/host2foreign.cpp                             
                            src/multimaster.cpp
                            src/relay_topic.cpp
                            src/relay_config.cpp
                            src/relay_manager.cpp
                            src/relay_tf.cpp)
target_link_libraries(host2foreign ${catkin_LIBRARIES} ${Boost_LIBRARIES})