#pragma once

#include "Packet.h"

/**
 * @brief Represents an interface for client network communication.
 */
class ClientNetworkInterface {
 public:
  /**
   * @brief Destructor (virtual to allow proper destruction of derived classes).
   */
  virtual ~ClientNetworkInterface() = default;

  /**
   * @brief Get the next packet to process
   * Need to delete the packet after using it
   * @return
   */
  virtual Packet* PopPacket() = 0;

  /**
   * @brief Send a packet to the server
   * @param packet The packet to send
   */
  virtual void SendPacket(Packet* packet) = 0;
};