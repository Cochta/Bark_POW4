#pragma once
#include <SFML/Network.hpp>
#include <mutex>
#include <queue>
#include <shared_mutex>

#include "packet.h"

/**
 * @brief Represents a client connected to the server.
 */

struct Client {
  sf::TcpSocket* socket = new sf::TcpSocket();  ///< Pointer to the TCP socket
                                                ///< associated with the client.
  std::queue<Packet*> packetsToBeSent =
      std::queue<Packet*>();  ///< Queue of packets to be sent to the client.
  mutable std::shared_mutex
      mutex_;  ///< Mutex to synchronize access to the packet queue.

  /**
   * @brief Sends a packet to the client.
   * @param packet Pointer to the packet to be sent.
   */
  void SendPacket(Packet* packet) {
    std::unique_lock lock(mutex_);
    packetsToBeSent.push(packet);
  }

  /**
   * @brief Checks if the packet queue is empty.
   * @return True if the packet queue is empty, false otherwise.
   */
  bool IsPacketsEmpty() const {
    std::shared_lock lock(mutex_);
    return packetsToBeSent.empty();
  }
};
