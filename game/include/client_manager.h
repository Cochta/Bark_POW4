#pragma once

#include <mutex>

#include "client.h"

class ClientManager {
  std::vector<Client*> clients;
  mutable std::mutex mutex;

 public:
  ClientManager() = default;

  ~ClientManager();

  bool isEmpty() const;

  std::size_t AddClient(sf::TcpSocket* socket);
  void RemoveClient(std::size_t index);

  /**
   * @brief Check packet to be sent for all clients
   */
  void CheckPacketToBeSent();
  void SendPacketToAllClients(Packet* packet, sf::TcpSocket* sender = nullptr);

  // void SendPacketToOneClient(sf::Packet* packet, sf::TcpSocket* client);

  Client* operator[](std::size_t index);
};