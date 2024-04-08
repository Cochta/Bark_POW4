#pragma once

#include <atomic>
#include <functional>

#include "client_manager.h"
#include "packet_manager.h"

/**
 * @brief Manages the server-side networking operations.
 */
class NetworkServerManager {
  ClientManager clients;  ///< Manager for handling connected clients.
  sf::TcpListener
      listener;  ///< TCP listener for accepting incoming connections.
  std::function<bool(sf::TcpSocket*, Packet*)>
      onClientMessageReceived;  ///< Callback function for processing client
                                ///< messages.
  mutable std::mutex mutex;     ///< Mutex for synchronization.

 public:
  /**
   * @brief Default constructor.
   */
  NetworkServerManager() = default;

  /**
   * @brief Destructor.
   */
  ~NetworkServerManager();

  std::atomic<bool> running =
      true;  ///< Flag indicating if the server is running.

  /**
   * @brief Binds the server to the specified port.
   * @param port The port number to bind to.
   * @return True if binding is successful, false otherwise.
   */
  bool Bind(unsigned short port);

  /**
   * @brief Listens to packets from clients.
   *        Returns false to disconnect the client.
   * @param onMessageReceived Callback function to be called when a packet is
   * received (other than acknowledgement and invalid).
   */
  void ListenToClientPackets(
      std::function<bool(sf::TcpSocket*, Packet*)> onMessageReceived);

  /**
   * @brief Starts the threads for the server to accept new clients and
   * send/receive packets.
   */
  void StartThreads();

  /**
   * @brief Receives a packet from a specific client.
   * @param clientIndex The index of the client to receive the packet from.
   */
  void ReceivePacketFromClient(std::size_t clientIndex);
};
