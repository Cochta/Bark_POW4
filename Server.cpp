
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Metrics.h"
#include "logger.h"
#include "network_server_manager.h"

struct Player {
  sf::TcpSocket* socket = nullptr;
  PlayerData data;
};

struct Lobby {
  Player p1, p2;
  bool gameStarted = false;
  void clear() {
    p1.socket = nullptr;
    p2.socket = nullptr;
    gameStarted = false;
    p1.data.clear();
    p2.data.clear();
  };
};
// Function to extract elo value from JSON string
int extractElo(const std::string& jsonString) {
  std::istringstream iss(jsonString);
  std::string token;
  std::vector<std::string> tokens;

  // Splitting the string by comma and colon to extract the elo value
  while (std::getline(iss, token, ',')) {
    tokens.push_back(token);
  }

  int elo = -1;  // Default value if not found

  // Searching for "elo" key and extracting its value
  for (const auto& t : tokens) {
    size_t found = t.find("elo");
    if (found != std::string::npos) {
      size_t colonPos = t.find(":");
      if (colonPos != std::string::npos) {
        std::string eloStr = t.substr(colonPos + 1);
        elo = std::stoi(eloStr);
        break;
      }
    }
  }

  return elo;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  std::vector<Lobby> lobbies;

  sf::Http http("127.0.0.1", 8000);

  NetworkServerManager server;

  // Bind the listener to a port
  if (!server.Bind(PORT)) {
    LOG_ERROR("Could not bind listener to port");
    return EXIT_FAILURE;
  }

  LOG("Server is listening to port " << PORT);

  server.ListenToClientPackets(
      [&lobbies, &http](sf::TcpSocket* socket, Packet* packet) {
        if (packet->type == PacketType::Connect) {
          auto& connectPacket = dynamic_cast<const ConnectPacket&>(*packet);
          sf::Http::Request postPlayer("/player", sf::Http::Request::Post);
          postPlayer.setBody(R"({"name": ")" + connectPacket.playerName +
                             R"(","elo": 300})");
          postPlayer.setField("Content-Type", "application/json");
          sf::Http::Response response = http.sendRequest(postPlayer);
          sf::Http::Request getPlayer("/player/" + connectPacket.playerName,
                                      sf::Http::Request::Get);
          response = http.sendRequest(getPlayer);

          for (auto& lobby : lobbies) {
            if (!lobby.gameStarted) {
              if (lobby.p1.socket == nullptr) {
                lobby.p1.socket = socket;
                lobby.p1.data.name = connectPacket.playerName;
                lobby.p1.data.elo = extractElo(response.getBody());
                std::cout << "p1 assigned to lobby" << std::endl;
                return true;
              } else if (lobby.p2.socket == nullptr) {
                lobby.p2.socket = socket;
                lobby.p2.data.name = connectPacket.playerName;
                lobby.p2.data.elo = extractElo(response.getBody());
                std::cout << "p2 assigned to lobby" << std::endl;
                lobby.gameStarted = true;
                auto startPacket = new StartGamePacket();
                startPacket->p1Name = lobby.p1.data.name;
                startPacket->p1Elo = lobby.p1.data.elo;
                startPacket->p2Name = lobby.p2.data.name;
                startPacket->p2Elo = lobby.p2.data.elo;
                PacketManager::SendPacket(*lobby.p1.socket, startPacket);
                PacketManager::SendPacket(*lobby.p2.socket, startPacket);
                auto firstPacket = new HasPlayedPacket();
                firstPacket->IsFirstTurn = true;
                PacketManager::SendPacket(*lobby.p1.socket, firstPacket);
                std::cout << "game started" << std::endl;
                return true;
              }
            }
          }

          // If no available lobby, create a new one
          Lobby newLobby;
          newLobby.p1.socket = socket;
          newLobby.p1.data.name = connectPacket.playerName;
          newLobby.p1.data.elo = extractElo(response.getBody());
          lobbies.push_back(newLobby);
          std::cout << "New lobby created" << std::endl;
          return true;
        }

        if (packet->type == PacketType::HasPlayed) {
          for (const auto& lobby : lobbies) {
            if (socket == lobby.p1.socket) {
              PacketManager::SendPacket(*lobby.p2.socket, packet);
            } else if (socket == lobby.p2.socket) {
              PacketManager::SendPacket(*lobby.p1.socket, packet);
            }
          }
        }
        if (packet->type == PacketType::GameFinished) {
          auto& endPacket = dynamic_cast<const GameFinishedPacket&>(*packet);
          for (auto& lobby : lobbies) {
            if (socket == lobby.p1.socket || socket == lobby.p2.socket) {
              if (!lobby.gameStarted) continue;
              lobby.gameStarted = false;
              LOG("Game finished");
              if (endPacket.HasP1Won) {
                sf::Http::Request updatePlayer("/player/" + lobby.p1.data.name,
                                               sf::Http::Request::Post);
                updatePlayer.setBody(R"({"elo": 30})");
                updatePlayer.setField("Content-Type", "application/json");
                sf::Http::Response response = http.sendRequest(updatePlayer);

                sf::Http::Request updatePlayer2("/player/" + lobby.p2.data.name,
                                                sf::Http::Request::Post);
                updatePlayer2.setBody(R"({"elo": -30})");
                updatePlayer2.setField("Content-Type", "application/json");
                response = http.sendRequest(updatePlayer2);
              } else {
                sf::Http::Request updatePlayer("/player/" + lobby.p1.data.name,
                                               sf::Http::Request::Post);
                updatePlayer.setBody(R"({"elo": -30})");
                updatePlayer.setField("Content-Type", "application/json");
                sf::Http::Response response = http.sendRequest(updatePlayer);

                sf::Http::Request updatePlayer2("/player/" + lobby.p2.data.name,
                                                sf::Http::Request::Post);
                updatePlayer2.setBody(R"({"elo": 30})");
                updatePlayer2.setField("Content-Type", "application/json");
                response = http.sendRequest(updatePlayer2);
              }

              lobby.clear();
            }
          }
        }
        if (packet->type == PacketType::Surrender) {
          for (auto& lobby : lobbies) {
            if (socket == lobby.p1.socket) {
              PacketManager::SendPacket(*lobby.p2.socket, packet);
            }
            if (socket == lobby.p2.socket) {
              PacketManager::SendPacket(*lobby.p1.socket, packet);
            }
          }
        }
        if (packet->type == PacketType::QuitLobby) {
          for (auto& lobby : lobbies) {
            if (socket == lobby.p1.socket) {
              lobby.p1.socket = nullptr;
              lobby.p1.data.clear();
            }
            if (socket == lobby.p2.socket) {
              lobby.p2.socket = nullptr;
              lobby.p1.data.clear();
            }
          }
        }

        return true;
      });
  server.StartThreads();

  while (server.running) {
    // sort players in lobbies
    for (size_t i = 1; i < lobbies.size(); i++) {
      if (lobbies[i].p1.socket != nullptr && lobbies[i].p2.socket == nullptr) {
        if (lobbies[i - 1].p1.socket == nullptr) {
          lobbies[i - 1].p1 = lobbies[i].p1;
          lobbies[i].clear();
        } else if (lobbies[i - 1].p2.socket == nullptr) {
          lobbies[i - 1].p2 = lobbies[i].p1;
          lobbies[i].clear();
        }
      } else if (lobbies[i].p2.socket != nullptr &&
                 lobbies[i].p1.socket == nullptr) {
        if (lobbies[i - 1].p1.socket == nullptr) {
          lobbies[i - 1].p1 = lobbies[i].p2;
          lobbies[i].clear();
        } else if (lobbies[i - 1].p2.socket == nullptr) {
          lobbies[i - 1].p2 = lobbies[i].p2;
          lobbies[i].clear();
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
