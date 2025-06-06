#pragma once
#include "test.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class TestClient {
 public:
  TestClient(std::shared_ptr<Channel> channel) : stub_(Test::NewStub(channel)) {}
  void TimeConsumingCall();

 private:
  std::unique_ptr<Test::Stub> stub_;
};
