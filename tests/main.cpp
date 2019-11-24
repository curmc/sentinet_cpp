#include "gtest/gtest.h"
#include <scpp/control/ZMQControlClient.hpp>
#include <scpp/kernel/KermitKernel.hpp>


namespace {

TEST(KermitTest, LifeCycle) {
  std::unique_ptr<scpp::curmt::KermitKernel> kernel;

  kernel = std::make_unique<scpp::curmt::KermitKernel>("cmd_vel", "command",
          "data", "realtime", true, true);

  kernel->start(std::chrono::milliseconds(10), std::chrono::seconds(10));

  kernel->kermit_quit();
}

}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
