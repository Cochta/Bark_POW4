#pragma once

#include <functional>
#include <thread>

#include "client.h"
#include "packet.h"

class NetworkClientManager {
 private:
  bool _running = true;
  std::queue<Packet*> _packetReceived;
  mutable std::shared_mutex _mutex;

  void ReceivePackets(Client& client);
  void SendPackets(Client& client) const;

 public:
  NetworkClientManager() = default;

  void StartThreads(Client& client);

  Packet* PopPacket();

  void Stop() { _running = false; }
};