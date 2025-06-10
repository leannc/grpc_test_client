#include <test.grpc.pb.h>

static int count = 0;
class MachineStatusReader : public ::grpc::ClientBidiReactor<::MachineStatusRequest, ::MachineStatusResponse> {
 public:
  MachineStatusReader(Test::Stub* stub) {
    count = 0;
    stub->async()->MachineStatus(&context_, this);
    NextWrite();
    StartRead(&response_);
    StartCall();
  }

  void OnReadDone(bool ok) override {
    if (ok) {
      // Process the received machine status
      std::cout << "【" << std::this_thread::get_id() << "】"
                << "Received machine status: " << response_.status() << std::endl;
      StartRead(&response_);
    } else {
      // Handle read error
    }
  }

  void OnWriteDone(bool ok) override {
    if (ok) {
      // Handle successful write
      NextWrite();
    } else {
      // Handle write error
    }
  }

  void OnDone(const ::grpc::Status& s) override {
    // Cleanup or final processing after reading all statuses
    std::unique_lock<std::mutex> l(mu_);
    done_ = true;
    cv_.notify_one();
  }

  ::grpc::Status Await() {
    std::unique_lock<std::mutex> l(mu_);
    cv_.wait(l, [this] { return done_; });
    return ::grpc::Status::OK;
  }

 private:
  void NextWrite() {
    if (count++ > 4) {
      std::cout << "【" << std::this_thread::get_id() << "】"
                << "Stopping further writes." << std::endl;
      StartWritesDone();
    }

    std::cout << "【" << std::this_thread::get_id() << "】"
              << "Preparing next write operation." << std::endl;

    StartWrite(&request_);
  }

  ::grpc::ClientContext context_;
  ::MachineStatusRequest request_;
  bool done_ = false;
  ::MachineStatusResponse response_;
  std::condition_variable cv_;
  std::mutex mu_;
};
