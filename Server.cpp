
#include "logger.h"
#include "network_server_manager.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  sf::TcpSocket* p1 = nullptr;
  sf::TcpSocket* p2 = nullptr;

  NetworkServerManager server;

  // Bind the listener to a port
  if (!server.Bind(PORT)) {
    LOG_ERROR("Could not bind listener to port");
    return EXIT_FAILURE;
  }

  LOG("Server is listening to port " << PORT);

  server.ListenToClientPackets(
      [&p1, &p2](sf::TcpSocket* socket, Packet* packet) {
        if (packet->type == PacketType::Connect) {
          if (p1 == nullptr) {
            p1 = socket;
            LOG("p1 assigned");
          } else if (p2 == nullptr) {
            p2 = socket;
            LOG("p2 assigned");
            auto startPacket = new StartGamePacket();
            PacketManager::SendPacket(*p1, startPacket);
            PacketManager::SendPacket(*p2, startPacket);
            auto fistPacket = new HasPlayedPacket();
            fistPacket->IsFirstTurn = true;
            PacketManager::SendPacket(*p1, fistPacket);
            LOG("game started");
          }
        }

        if (packet->type == PacketType::HasPlayed) {
          if (socket == p1) {
            PacketManager::SendPacket(*p2, packet);
          } else if (socket == p2) {
            PacketManager::SendPacket(*p1, packet);
          }
        }
        if (packet->type == PacketType::GameFinished) {
          printf("ouafouaf");
          p1 = nullptr;
          p2 = nullptr;
        }

        return true;
      });
  server.StartThreads();

  while (server.running) {
  }

  return EXIT_SUCCESS;
}
