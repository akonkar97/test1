#include <chrono>
#include <memory>

#include "gtest/gtest.h"

#include "rclcpp/exceptions.hpp"
#include "rclcpp/rclcpp.hpp"

#include "cpp_pubsub/srv/add_two_ints.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

class CLASSNAME : public ::testing::Test
{
public:
  static void SetUpTestCase()
  {
    rclcpp::init(0, nullptr);
  }

  static void TearDownTestCase()
  {
    rclcpp::shutdown();
  }
};

TEST_F(CLASSNAME, test_add_noreqid) {

    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("add_two_ints_client_test");
    rclcpp::Client<cpp_pubsub::srv::AddTwoInts>::SharedPtr client =
        node->create_client<cpp_pubsub::srv::AddTwoInts>("add_two_ints");

    auto request = std::make_shared<cpp_pubsub::srv::AddTwoInts::Request>();
    request->a = 1;
    request->b = 2;

     if (!client->wait_for_service(20s)) {
    ASSERT_TRUE(false) << "service not available after waiting";
     }

    auto result = client->async_send_request(request);
    // Wait for the result.
    if (rclcpp::spin_until_future_complete(node, result) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Sum: %f", result.get()->sum);
    } else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service add_two_ints");
    }


  auto ret = rclcpp::spin_until_future_complete(node, result, 5s);  // Wait for the result.
  ASSERT_EQ(ret, rclcpp::FutureReturnCode::SUCCESS);

  EXPECT_EQ(3, result.get()->sum);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}