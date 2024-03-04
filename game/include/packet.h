#pragma once

#include <SFML/Network.hpp>

enum class PacketType {
  Connect,
  StartGame,
  HasPlayed,
  Disconnect,
  Message,
  Acknowledgement,
  Invalid
};

struct ConnectPacket {
  std::string playerName;
};

struct DisconnectPacket {
  std::string playerName;
};

struct MessagePacket {
  std::string playerName;
  std::string message;
};

struct StartGamePacket {
  bool start = true;
};
struct HasPlayedPacket {
  bool IsFirstTurn = true;
  float X = 0, Y = 0;
};

sf::Packet& operator<<(sf::Packet& packet, const ConnectPacket& message);
sf::Packet& operator>>(sf::Packet& packet, ConnectPacket& message);

sf::Packet& operator<<(sf::Packet& packet, const StartGamePacket& content);
sf::Packet& operator>>(sf::Packet& packet, StartGamePacket& content);

sf::Packet& operator<<(sf::Packet& packet, const HasPlayedPacket& content);
sf::Packet& operator>>(sf::Packet& packet, HasPlayedPacket& content);

sf::Packet& operator<<(sf::Packet& packet, const DisconnectPacket& message);
sf::Packet& operator>>(sf::Packet& packet, DisconnectPacket& message);

sf::Packet& operator<<(sf::Packet& packet, const MessagePacket& message);
sf::Packet& operator>>(sf::Packet& packet, MessagePacket& message);
