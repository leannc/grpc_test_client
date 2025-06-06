#include "TestClient.h"

void TestClient::TimeConsumingCall() {
  TimeConsumingRequest request;
  TimeConsumingResponse response;
  ::grpc::ClientContext context;
  std::mutex mu;
  std::condition_variable cv;
  bool done1 = false;
  bool done2 = false;
  bool done3 = false;

  std::cout << "【" << std::this_thread::get_id() << "】" << "before calling【TimeConsumingMethod】" << std::endl;
  stub_->async()->TimeConsumingMethod(&context, &request, &response, [&](::grpc::Status status) {
    auto res = response.status();
    std::cout << "【" << std::this_thread::get_id() << "】" << "in the callback function, status is " << res
              << std::endl;
    std::lock_guard<std::mutex> lock(mu);
    done1 = true;  // 保证了wait的正常返回
    cv.notify_one();
    std::cout << "【" << std::this_thread::get_id() << "】" << "callback done" << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  TimeConsumingRequest request2;
  TimeConsumingResponse response2;
  ::grpc::ClientContext context2;
  stub_->async()->TimeConsumingMethod(&context2, &request2, &response2, [&](::grpc::Status status) {
    auto res = response2.status();
    std::cout << "【" << std::this_thread::get_id() << "】" << "in the callback function, status is " << res
              << std::endl;
    std::lock_guard<std::mutex> lock(mu);
    done2 = true;  // 保证了wait的正常返回
    cv.notify_one();
    std::cout << "【" << std::this_thread::get_id() << "】" << "callback done" << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  TimeConsumingRequest request3;
  TimeConsumingResponse response3;
  ::grpc::ClientContext context3;
  stub_->async()->TimeConsumingMethod(&context3, &request3, &response3, [&](::grpc::Status status) {
    auto res = response3.status();
    std::cout << "【" << std::this_thread::get_id() << "】" << "in the callback function, status is " << res
              << std::endl;
    std::lock_guard<std::mutex> lock(mu);
    done3 = true;  // 保证了wait的正常返回
    cv.notify_one();
    std::cout << "【" << std::this_thread::get_id() << "】" << "callback done" << std::endl;
  });
  std::cout << "【" << std::this_thread::get_id() << "】"
            << "after calling【TimeConsumingMethod】, waiting for call return" << std::endl;
  //   std::this_thread::sleep_for(std::chrono::seconds(8));
  std::unique_lock<std::mutex> lock(mu);
  cv.wait(lock, [&done1, &done2, &done3] {
    if ((done1 && done2 && done3) == false) { std::cout << "not all done" << std::endl; }
    return done1 && done2 && done3;
  });  // 这个done很重要，保证了wait就算在notify之后被调用，也能正常返回。
}
