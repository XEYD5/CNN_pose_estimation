// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from ros2_aruco_interfaces:msg/ArucoMarkers.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "ros2_aruco_interfaces/msg/detail/aruco_markers__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace ros2_aruco_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void ArucoMarkers_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) ros2_aruco_interfaces::msg::ArucoMarkers(_init);
}

void ArucoMarkers_fini_function(void * message_memory)
{
  auto typed_message = static_cast<ros2_aruco_interfaces::msg::ArucoMarkers *>(message_memory);
  typed_message->~ArucoMarkers();
}

size_t size_function__ArucoMarkers__marker_ids(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<int64_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ArucoMarkers__marker_ids(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<int64_t> *>(untyped_member);
  return &member[index];
}

void * get_function__ArucoMarkers__marker_ids(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<int64_t> *>(untyped_member);
  return &member[index];
}

void resize_function__ArucoMarkers__marker_ids(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<int64_t> *>(untyped_member);
  member->resize(size);
}

size_t size_function__ArucoMarkers__poses(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<geometry_msgs::msg::Pose> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ArucoMarkers__poses(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<geometry_msgs::msg::Pose> *>(untyped_member);
  return &member[index];
}

void * get_function__ArucoMarkers__poses(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<geometry_msgs::msg::Pose> *>(untyped_member);
  return &member[index];
}

void resize_function__ArucoMarkers__poses(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<geometry_msgs::msg::Pose> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ArucoMarkers_message_member_array[4] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_aruco_interfaces::msg::ArucoMarkers, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "detected",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_aruco_interfaces::msg::ArucoMarkers, detected),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "marker_ids",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_aruco_interfaces::msg::ArucoMarkers, marker_ids),  // bytes offset in struct
    nullptr,  // default value
    size_function__ArucoMarkers__marker_ids,  // size() function pointer
    get_const_function__ArucoMarkers__marker_ids,  // get_const(index) function pointer
    get_function__ArucoMarkers__marker_ids,  // get(index) function pointer
    resize_function__ArucoMarkers__marker_ids  // resize(index) function pointer
  },
  {
    "poses",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Pose>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_aruco_interfaces::msg::ArucoMarkers, poses),  // bytes offset in struct
    nullptr,  // default value
    size_function__ArucoMarkers__poses,  // size() function pointer
    get_const_function__ArucoMarkers__poses,  // get_const(index) function pointer
    get_function__ArucoMarkers__poses,  // get(index) function pointer
    resize_function__ArucoMarkers__poses  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ArucoMarkers_message_members = {
  "ros2_aruco_interfaces::msg",  // message namespace
  "ArucoMarkers",  // message name
  4,  // number of fields
  sizeof(ros2_aruco_interfaces::msg::ArucoMarkers),
  ArucoMarkers_message_member_array,  // message members
  ArucoMarkers_init_function,  // function to initialize message memory (memory has to be allocated)
  ArucoMarkers_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ArucoMarkers_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ArucoMarkers_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace ros2_aruco_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ros2_aruco_interfaces::msg::ArucoMarkers>()
{
  return &::ros2_aruco_interfaces::msg::rosidl_typesupport_introspection_cpp::ArucoMarkers_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros2_aruco_interfaces, msg, ArucoMarkers)() {
  return &::ros2_aruco_interfaces::msg::rosidl_typesupport_introspection_cpp::ArucoMarkers_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
