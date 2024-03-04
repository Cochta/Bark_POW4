
#include "logger.h"
#include "network_server_manager.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  NetworkServerManager server;

  const int PORT = 55555;

  sf::TcpSocket* p1 = nullptr;
  sf::TcpSocket* p2 = nullptr;

  // Bind the listener to a port
  if (!server.Bind(PORT)) {
    LOG_ERROR("Could not bind listener to port");
    return EXIT_FAILURE;
  }

  LOG("Server is listening to port " << PORT);

  server.ListenToClientPackets([&server, &p1, &p2](sf::TcpSocket* socket,
                                                   const PacketType packetType,
                                                   sf::Packet packet) {
    if (packetType == PacketType::Connect) {
      ConnectPacket connectPacket = PacketManager::GetConnectPacket(packet);

      if (p1 == nullptr) {
        p1 = socket;
        LOG("p1 assigned");
      } else if (p2 == nullptr) {
        p2 = socket;
        LOG("p2 assigned");
        server.StartGame(StartGamePacket());
        auto packet = HasPlayedPacket();
        packet.IsFirstTurn = true;
        server.SendHasPlayedToOneClient(packet, p1);
        LOG("game started");
      }
    }

    if (packetType == PacketType::HasPlayed) {
      HasPlayedPacket hasPlayedPacket =
          PacketManager::GetHasPlayedPacket(packet);
      if (socket == p1) {
        server.SendHasPlayedToOneClient(hasPlayedPacket, p2);
      } else if (socket == p2) {
        server.SendHasPlayedToOneClient(hasPlayedPacket, p1);
      }
    }

    return true;
  });

  server.StartThreads();

  while (server.running) {
    std::string input;
    std::getline(std::cin, input);

    if (input == "stop") {
      server.running = false;
      break;
    } else {
      server.SendMessageToAllClients(MessagePacket{"Server", input});
    }
  }

  return EXIT_SUCCESS;
}
