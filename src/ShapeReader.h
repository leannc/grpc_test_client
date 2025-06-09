#include <test.grpc.pb.h>

class ShapeReader : public ::grpc::ClientReadReactor<::Shape> {
 public:
  ShapeReader(Test::Stub* stub) {
    ::SceneID scene_id;
    scene_id.set_id(12345);  // Example scene ID, replace with actual logic
    stub->async()->GetShapes(&context_, &scene_id, this);
    StartRead(&shape_);
    StartCall();
  }

  void OnReadDone(bool ok) override {
    if (ok) {
      // Process the received shape
      std::cout << "Received shape: " << shape_.type() << std::endl;
      StartRead(&shape_);
    } else {
      // Handle read error
    }
  }

  void OnDone(const ::grpc::Status& s) override {
    // Cleanup or final processing after reading all shapes
    std::unique_lock<std::mutex> l(mu_);
    status_ = s;
    done_ = true;
    cv_.notify_one();
  }

  ::grpc::Status Await() {
    std::unique_lock<std::mutex> l(mu_);
    cv_.wait(l, [this] { return done_; });
    return std::move(status_);
  }

 private:
  ::grpc::ClientContext context_;
  bool done_ = false;
  ::Shape shape_;
  std::condition_variable cv_;
  ::grpc::Status status_;
  std::mutex mu_;
};
