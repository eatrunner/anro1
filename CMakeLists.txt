cmake_minimum_required(VERSION 2.8.3)
project(anro1)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  genmsg
)


 add_message_files(
   DIRECTORY
   FILES
   mapMessage.msg
   car.msg
   lightsVector.msg
   light.msg
   turn.msg
   turnsVector.msg
 )


 generate_messages(
   DEPENDENCIES
   std_msgs
 )






catkin_package(
  INCLUDE_DIRS include
  LIBRARIES anro1
  CATKIN_DEPENDS message_runtime roscpp rospy std_msgs
  DEPENDS system_lib
)


include_directories(
  ${catkin_INCLUDE_DIRS}
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(MapOfCity src/MapOfCity.cpp)
target_link_libraries(MapOfCity ${catkin_LIBRARIES})
add_dependencies(MapOfCity anro1_generate_messages_cpp)


add_executable(Car src/Car.cpp)
target_link_libraries(Car ${catkin_LIBRARIES})
add_dependencies(Car anro1_generate_messages_cpp)

add_executable(Model src/Model.cpp)
target_link_libraries(Model ${catkin_LIBRARIES})

add_executable(turns src/turns.cpp)
target_link_libraries(turns ${catkin_LIBRARIES})

