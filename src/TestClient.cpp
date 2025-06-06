#include "TestClient.h"

void TestClient::TimeConsumingCall() {
  TimeConsumingRequest request;
  TimeConsumingResponse response;
  ::grpc::ClientContext context;
  std::mutex mu;
  std::condition_variable cv;
  bool done = false;

  std::cout << "before calling【TimeConsumingMethod】" << std::endl;
  stub_->async()->TimeConsumingMethod(&context, &request, &response, [&](::grpc::Status status) {
    auto res = response.status();
    std::cout << "in the callback function, status is " << res << std::endl;
    std::lock_guard<std::mutex> lock(mu);
    done = true;
    cv.notify_one();
  });

  std::cout << "after calling【TimeConsumingMethod】, waiting for call return" << std::endl;
  std::unique_lock<std::mutex> lock(mu);
  cv.wait(lock, [&done] { return done; });
}
