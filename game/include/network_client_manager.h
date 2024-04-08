#pragma once

#include <queue>
#include <shared_mutex>

#include "Packet.h"
#include "client_network_interface.h"

/**
 * @brief Handles receiving and sending packets from/to the server.
 */
class NetworkClientManager final : public ClientNetworkInterface {
 private:
  sf::TcpSocket* _socket =
      new sf::TcpSocket();              ///< Pointer to the TCP socket used for
                                        ///< communication with the server.
  std::queue<Packet*> _packetReceived;  ///< Queue of received packets.
  mutable std::shared_mutex _receivedMutex;  ///< Mutex to synchronize access to
                                             ///< the received packet queue.
  std::queue<Packet*>
      _packetToSend;  ///< Queue of packets to send to the server.
  mutable std::shared_mutex
      _sendMutex;  ///< Mutex to synchronize access to the packet to send queue.
  bool _running = true;  ///< Flag indicating if the client manager is running.

  /**
   * @brief Launches a thread to receive packets from the server.
   */
  void ReceivePackets();

  /**
   * @brief Launches a thread to send packets to the server.
   */
  void SendPackets();

  /**
   * @brief Checks if the received packet queue is empty.
   * @return True if the received packet queue is empty, false otherwise.
   */
  bool IsPacketReceivedEmpty() const {
    std::shared_lock lock(_receivedMutex);
    return _packetReceived.empty();
  }

  /**
   * @brief Checks if the packet to send queue is empty.
   * @return True if the packet to send queue is empty, false otherwise.
   */
  bool IsPacketToSendEmpty() const {
    std::shared_lock lock(_sendMutex);
    return _packetToSend.empty();
  }

 public:
  bool IsConnected =
      false;  ///< Flag indicating if the client is connected to the server.

  /**
   * @brief Constructs a new NetworkClientManager object.
   * @param host The host address of the server.
   * @param port The port number of the server.
   */
  NetworkClientManager(std::string_view host, unsigned short port);

  /**
   * @brief Retrieves the next packet to process.
   *        The caller is responsible for deleting the returned packet after
   * use.
   * @return Pointer to the packet to process.
   */
  Packet* PopPacket() override;

  /**
   * @brief Sends a packet to the server.
   * @param packet Pointer to the packet to send.
   */
  void SendPacket(Packet* packet) override;

  /**
   * @brief Stops the client manager.
   */
  void Stop();
};