#pragma once

#include <mutex>

#include "client.h"

/**
 * @brief Manages multiple clients connected to the server.
 */
class ClientManager {
  std::vector<Client*>
      clients;  ///< Vector holding pointers to connected clients.
  mutable std::mutex
      mutex;  ///< Mutex to synchronize access to the clients vector.

 public:
  /**
   * @brief Default constructor.
   */
  ClientManager() = default;

  /**
   * @brief Destructor.
   */
  ~ClientManager();

  /**
   * @brief Checks if the client manager is empty.
   * @return True if the client manager is empty, false otherwise.
   */
  bool isEmpty() const;

  /**
   * @brief Adds a client to the manager.
   * @param socket Pointer to the TCP socket associated with the client.
   * @return The index of the added client in the clients vector.
   */
  std::size_t AddClient(sf::TcpSocket* socket);

  /**
   * @brief Removes a client from the manager.
   * @param index The index of the client to be removed.
   */
  void RemoveClient(std::size_t index);

  /**
   * @brief Checks packets to be sent for all clients.
   */
  void CheckPacketToBeSent();

  /**
   * @brief Sends a packet to all clients.
   * @param packet Pointer to the packet to be sent.
   * @param sender Pointer to the TCP socket of the sender client (optional).
   */
  void SendPacketToAllClients(Packet* packet, sf::TcpSocket* sender = nullptr);

  /**
   * @brief Accesses a client in the manager by index.
   * @param index The index of the client to access.
   * @return Pointer to the client at the specified index.
   */
  Client* operator[](std::size_t index);
};