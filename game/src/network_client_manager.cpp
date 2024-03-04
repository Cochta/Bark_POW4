#include "network_client_manager.h"

#include "logger.h"
#include "packet_manager.h"

void NetworkClientManager::ReceivePackets(Client& client) {
  while (running) {
    sf::Packet packet;
    PacketType packetType =
        PacketManager::ReceivePacket(*client.socket, packet);

    if (packetType == PacketType::Invalid) {
      LOG_ERROR("Could not receive packet");
      std::exit(EXIT_FAILURE);
    }

    if (onServerPacketReceived) {
      running = onServerPacketReceived(packet, packetType);
    }
  }
}

void NetworkClientManager::SendPackets(Client& client) const {
  while (running) {
    if (!client.packetsToBeSent.empty()) {
      sf::Packet* packet = client.packetsToBeSent.front();

      client.packetsToBeSent.pop();

      client.socket->send(*packet);

      delete packet;
    }
  }
}

void NetworkClientManager::SetOnMessageReceived(
    std::function<bool(sf::Packet&, PacketType)> onMessageReceived) {
  this->onServerPacketReceived = std::move(onMessageReceived);
}

void NetworkClientManager::StartThreads(Client& client) {
  std::thread receiveThread(&NetworkClientManager::ReceivePackets, this,
                            std::ref(client));
  receiveThread.detach();

  std::thread sendThread(&NetworkClientManager::SendPackets, this,
                         std::ref(client));
  sendThread.detach();
}
