#include "TestClient.h"
#include "ShapeReader.h"

void TestClient::TimeConsumingCall() {
  std::mutex mu;
  std::condition_variable cv;
  std::vector<bool> results;

  std::cout << "【" << std::this_thread::get_id() << "】" << "before calling【TimeConsumingMethod】" << std::endl;
  for (int i = 0; i < 5; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    TimeConsumingRequest request;
    TimeConsumingResponse response;
    ::grpc::ClientContext context;

    int index = results.size();
    results.push_back(false);

    // async call
    stub_->async()->TimeConsumingMethod(
        &context, &request, &response, [&results, &response, &mu, &cv, index](::grpc::Status status) {
          auto res = response.status();
          std::cout << "【" << std::this_thread::get_id() << "】" << "in the callback function, status is " << res
                    << std::endl;
          std::lock_guard<std::mutex> lock(mu);
          results[index] = true;  // 保证了wait的正常返回
          cv.notify_one();
          std::cout << "【" << std::this_thread::get_id() << "】" << "callback done" << std::endl;
        });
  }

  std::cout << "【" << std::this_thread::get_id() << "】"
            << "after calling【TimeConsumingMethod】, waiting for call return" << std::endl;
  //   std::this_thread::sleep_for(std::chrono::seconds(8));
  std::unique_lock<std::mutex> lock(mu);

  cv.wait(lock, [&results] {
    bool all_true = std::all_of(results.begin(), results.end(), [](bool b) { return b; });
    if (all_true == false) {
      std::cout << "not all done" << std::endl;
    }  // 这里可以打印出当前的results状态
    else {
      std::cout << "all done" << std::endl;
    }
    return all_true;
  });  // 这个done很重要，保证了wait就算在notify之后被调用，也能正常返回。
}

void TestClient::GetShape_ServerStream() {
  ShapeReader shape_reader(stub_.get());
  auto status = shape_reader.Await();
  if (status.ok()) {
    std::cout << "Received all shapes successfully." << std::endl;
  } else {
    std::cerr << "Failed to receive shapes: " << status.error_message() << std::endl;
  }
}
