#pragma once

#include <grpcpp/grpcpp.h>
#include <test.grpc.pb.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class TestClient {
 public:
  TestClient(std::shared_ptr<Channel> channel) : stub_(Test::NewStub(channel)) {}
  void TimeConsumingCall();
  void GetShape_ServerStream();

 private:
  std::unique_ptr<Test::Stub> stub_;
};
