
#include "logger.h"
#include "network_server_manager.h"

struct Lobby {
  sf::TcpSocket* p1 = nullptr;
  sf::TcpSocket* p2 = nullptr;
  bool gameStarted = false;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  std::vector<Lobby> lobbies;
  // sf::TcpSocket* p1 = nullptr;
  // sf::TcpSocket* p2 = nullptr;

  NetworkServerManager server;

  // Bind the listener to a port
  if (!server.Bind(PORT)) {
    LOG_ERROR("Could not bind listener to port");
    return EXIT_FAILURE;
  }

  LOG("Server is listening to port " << PORT);

  server.ListenToClientPackets(
      [&lobbies](sf::TcpSocket* socket, Packet* packet) {
        if (packet->type == PacketType::Connect) {
          for (auto& lobby : lobbies) {
            if (!lobby.gameStarted) {
              if (lobby.p1 == nullptr) {
                lobby.p1 = socket;
                std::cout << "p1 assigned to lobby" << std::endl;
                return true;
              } else if (lobby.p2 == nullptr) {
                lobby.p2 = socket;
                std::cout << "p2 assigned to lobby" << std::endl;
                lobby.gameStarted = true;
                auto startPacket = new StartGamePacket();
                PacketManager::SendPacket(*lobby.p1, startPacket);
                PacketManager::SendPacket(*lobby.p2, startPacket);
                auto firstPacket = new HasPlayedPacket();
                firstPacket->IsFirstTurn = true;
                PacketManager::SendPacket(*lobby.p1, firstPacket);
                std::cout << "game started" << std::endl;
                return true;
              }
            }
          }

          // If no available lobby, create a new one
          Lobby newLobby;
          newLobby.p1 = socket;
          lobbies.push_back(newLobby);
          std::cout << "New lobby created" << std::endl;
          return true;
        }

        if (packet->type == PacketType::HasPlayed) {
          for (const auto& lobby : lobbies) {
            if (socket == lobby.p1) {
              PacketManager::SendPacket(*lobby.p2, packet);
            } else if (socket == lobby.p2) {
              PacketManager::SendPacket(*lobby.p1, packet);
            }
          }
        }
        if (packet->type == PacketType::GameFinished) {
          for (auto& lobby : lobbies) {
            if (socket == lobby.p1 || socket == lobby.p2) {
              LOG("Game finished");
              lobby.p1 = nullptr;
              lobby.p2 = nullptr;
              lobby.gameStarted = false;
            }
          }
        }
        if (packet->type == PacketType::Surrender) {
          for (auto& lobby : lobbies) {
            if (socket == lobby.p1) {
              PacketManager::SendPacket(*lobby.p2, packet);
            }
            if (socket == lobby.p2) {
              PacketManager::SendPacket(*lobby.p1, packet);
            }
          }
        }
        if (packet->type == PacketType::QuitLobby) {
          for (auto& lobby : lobbies) {
            if (socket == lobby.p1) {
              lobby.p1 = nullptr;
            }
            if (socket == lobby.p2) {
              lobby.p1 = nullptr;
            }
          }
        }

        return true;
      });
  server.StartThreads();

  while (server.running) {
    // sort players in lobbies
    for (size_t i = 1; i < lobbies.size(); i++) {
      if (lobbies[i].p1 != nullptr && lobbies[i].p2 == nullptr) {
        if (lobbies[i - 1].p1 == nullptr) {
          lobbies[i - 1].p1 = lobbies[i].p1;
          lobbies[i].p1 = nullptr;
        } else if (lobbies[i - 1].p2 == nullptr) {
          lobbies[i - 1].p2 = lobbies[i].p1;
          lobbies[i].p1 = nullptr;
        }
      } else if (lobbies[i].p2 != nullptr && lobbies[i].p1 == nullptr) {
        if (lobbies[i - 1].p1 == nullptr) {
          lobbies[i - 1].p1 = lobbies[i].p2;
          lobbies[i].p2 = nullptr;
        } else if (lobbies[i - 1].p2 == nullptr) {
          lobbies[i - 1].p2 = lobbies[i].p2;
          lobbies[i].p2 = nullptr;
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
