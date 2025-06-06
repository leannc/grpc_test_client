//
// Created by wangkang on 2023/12/3.
//
#include "TestClient.h"
#include <iostream>
#include <thread>

int main() {
  TestClient client(grpc::CreateChannel("localhost:2105", grpc::InsecureChannelCredentials()));

  client.TimeConsumingCall();

  //   std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "grpc client finished!" << std::endl;
  return 0;
}
