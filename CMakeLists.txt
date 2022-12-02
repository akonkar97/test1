cmake_minimum_required(VERSION 3.8)
project(cpp_pubsub)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rosbag2_cpp REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/AddTwoInts.srv"
)
ament_export_dependencies(rosidl_default_runtime)

add_executable(talker src/publisher_member_function.cpp)
ament_target_dependencies(talker rclcpp std_msgs)

add_executable(listener src/subscriber_member_function.cpp)
ament_target_dependencies(listener rclcpp std_msgs)

add_executable(server_s src/add_two_ints_server.cpp)
ament_target_dependencies(server_s rclcpp)

add_executable(client_s src/add_two_ints_client.cpp)
ament_target_dependencies(client_s rclcpp)

rosidl_target_interfaces(server_s
  ${PROJECT_NAME} "rosidl_typesupport_cpp")
rosidl_target_interfaces(client_s
  ${PROJECT_NAME} "rosidl_typesupport_cpp")

install(TARGETS
  talker
  listener
  server_s
  client_s
  DESTINATION lib/${PROJECT_NAME})
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  find_package(ament_lint_auto REQUIRED)

  set(ament_cmake_cppcheck_FOUND TRUE)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  set(ament_cmake_flake8_FOUND TRUE)
  set(ament_cmake_uncrustify_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
  set(TESTFILES
    test/test.cpp
    server_s)
  ament_add_gtest(${PROJECT_NAME}_test ${TESTFILES})

  rosidl_target_interfaces(${PROJECT_NAME}_test
  ${PROJECT_NAME} "rosidl_typesupport_cpp")

  ament_target_dependencies(${PROJECT_NAME}_test rclcpp)
  install(TARGETS
  ${PROJECT_NAME}_test
  DESTINATION lib/${PROJECT_NAME})

endif()

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS})
ament_package()
