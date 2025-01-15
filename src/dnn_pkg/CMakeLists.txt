cmake_minimum_required(VERSION 3.5)
project(dnn_pkg)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
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

# Declare the executable
add_executable(dnn_exe src/dnn_code.cpp)
ament_target_dependencies(dnn_exe rclcpp cv_bridge sensor_msgs)

# Link libraries
target_link_libraries(dnn_exe ${DEPENDENCIES})

# Set target properties
set_target_properties(dnn_exe PROPERTIES
    FOLDER "Examples/OpenCV"
)

install(
    TARGETS dnn_exe
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()
