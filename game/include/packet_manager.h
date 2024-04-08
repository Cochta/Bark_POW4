#pragma once

#include "logger.h"
#include "packet.h"

/**
 * @brief Namespace for managing packets.
 */
namespace PacketManager {

/**
 * @brief Sends a packet over a TCP socket.
 * @param socket Reference to the TCP socket.
 * @param packet Pointer to the packet to send.
 * @return True if the packet is sent successfully, false otherwise.
 */
bool SendPacket(sf::TcpSocket& socket, Packet* packet);

/**
 * @brief Receives a packet from a TCP socket.
 * @param socket Reference to the TCP socket to receive the packet from.
 * @return Pointer to the received packet.
 */
Packet* ReceivePacket(sf::TcpSocket& socket);

/**
 * @brief Creates an sf::Packet from a Packet structure.
 * @param packet Pointer to the packet to be converted.
 * @return Pointer to the created sf::Packet.
 */
sf::Packet* CreatePacket(Packet* packet);

}  // namespace PacketManager