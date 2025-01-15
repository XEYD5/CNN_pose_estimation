cmake_minimum_required(VERSION 3.5)
project(cnn_pkg)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
  add_compile_options(-Wno-pedantic)
  add_compile_options(-Wno-unused-parameter)
  add_compile_options(-Wno-unused-but-set-variable)
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Find OpenCV
find_package(OpenCV REQUIRED)
set(DEPENDENCIES realsense2 ${OpenCV_LIBS})

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(ros2_aruco_interfaces REQUIRED)

# Add TensorFlow C++ API
#set(TENSORFLOW_INCLUDE_DIR "/home/jorge/.local/lib/python3.8/site-packages/tensorflow/include")
#set(TENSORFLOW_LIB_DIR "/home/jorge/.local/lib/python3.8/site-packages/tensorflow")
include_directories("/home/jorge/.local/lib/python3.8/site-packages/tensorflow/include")
link_directories("/home/jorge/.local/lib/python3.8/site-packages/tensorflow")
#include_directories(/usr/local/include)
#link_directories(/usr/local/lib)

# Declare the executables
add_executable(get_frames_exe src/get_frames_code.cpp)
ament_target_dependencies(get_frames_exe rclcpp cv_bridge sensor_msgs)
add_executable(get_frames2_exe src/get_frames_code2.cpp)
ament_target_dependencies(get_frames2_exe rclcpp cv_bridge sensor_msgs)
add_executable(send_frames_exe src/send_frames_code.cpp)
ament_target_dependencies(send_frames_exe rclcpp cv_bridge sensor_msgs)
add_executable(get_aruco_exe src/get_aruco_code.cpp)
ament_target_dependencies(get_aruco_exe rclcpp cv_bridge sensor_msgs ros2_aruco_interfaces)
add_executable(sort_dataset_exe src/sort_dataset_code.cpp)
ament_target_dependencies(sort_dataset_exe rclcpp cv_bridge sensor_msgs)
#add_executable(cnn_exe src/cnn_code.cpp)
#ament_target_dependencies(cnn_exe rclcpp cv_bridge sensor_msgs)# tensorflow_cc tensorflow_framework)

# Link libraries
target_link_libraries(get_frames_exe ${DEPENDENCIES})
target_link_libraries(get_frames2_exe ${DEPENDENCIES})
target_link_libraries(send_frames_exe ${DEPENDENCIES})
target_link_libraries(get_aruco_exe ${DEPENDENCIES})
target_link_libraries(sort_dataset_exe ${DEPENDENCIES})
#target_link_libraries(cnn_exe ${DEPENDENCIES})# tensorflow)


# Set target properties
set_target_properties(get_frames_exe PROPERTIES
    FOLDER "Examples/OpenCV"
)
set_target_properties(get_frames2_exe PROPERTIES
    FOLDER "Examples/OpenCV"
)
set_target_properties(send_frames_exe PROPERTIES
    FOLDER "Examples/OpenCV"
)
set_target_properties(get_aruco_exe PROPERTIES
    FOLDER "Examples/OpenCV"
)
set_target_properties(sort_dataset_exe PROPERTIES
    FOLDER "Examples/OpenCV"
)
set_target_properties(sort_dataset_exe PROPERTIES
    FOLDER "Examples/OpenCV"
)

# Install targets
install(
    TARGETS get_frames_exe get_frames2_exe send_frames_exe get_aruco_exe sort_dataset_exe #cnn_exe #_exe
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()