#pragma once

#include <SFML/Network.hpp>
#include "Metrics.h"

enum class PacketType {
  Connect,
  StartGame,
  HasPlayed,
  GameFinished,
  Surrender,
  QuitLobby,
  Invalid
};
struct Packet {
  Packet() = default;
  explicit Packet(PacketType type) : type(type) {}
  virtual ~Packet() = default;

  PacketType type = PacketType::Invalid;

  static Packet* FromType(PacketType type);
};

struct ConnectPacket final : Packet {
  ConnectPacket() : Packet(PacketType::Connect) {}
  explicit ConnectPacket(std::string_view playerName)
      : Packet(PacketType::Connect), playerName(playerName) {}

  std::string playerName;
};

struct StartGamePacket final : Packet {
  StartGamePacket() : Packet(PacketType::StartGame) {}
  std::string p1Name;
  int p1Elo;
  std::string p2Name;
  int p2Elo;
};
struct HasPlayedPacket final : Packet {
  HasPlayedPacket() : Packet(PacketType::HasPlayed) {}
  bool IsFirstTurn = true;
  float X = 0, Y = 0;
  int index = 0;
};

struct GameFinishedPacket final : Packet {
  GameFinishedPacket() : Packet(PacketType::GameFinished) {}
  bool HasP1Won = true;
};

struct SurrenderPacket final : Packet {
  SurrenderPacket() : Packet(PacketType::Surrender) {}
  bool hasSurrendered = true;
};

struct QuitLobbyPacket final : Packet {
  QuitLobbyPacket() : Packet(PacketType::QuitLobby) {}
  bool hasQuit = true;
};

struct InvalidPacket final : Packet {
  InvalidPacket() : Packet(PacketType::Invalid) {}
};

sf::Packet& operator<<(sf::Packet& packet, const Packet& packetType);
sf::Packet& operator>>(sf::Packet& packet, Packet& packetType);

sf::Packet& operator<<(sf::Packet& packet, const ConnectPacket& message);
sf::Packet& operator>>(sf::Packet& packet, ConnectPacket& message);

sf::Packet& operator<<(sf::Packet& packet, const StartGamePacket& content);
sf::Packet& operator>>(sf::Packet& packet, StartGamePacket& content);

sf::Packet& operator<<(sf::Packet& packet, const HasPlayedPacket& content);
sf::Packet& operator>>(sf::Packet& packet, HasPlayedPacket& content);

sf::Packet& operator<<(sf::Packet& packet, const GameFinishedPacket& content);
sf::Packet& operator>>(sf::Packet& packet, GameFinishedPacket& content);

sf::Packet& operator<<(sf::Packet& packet, const SurrenderPacket& content);
sf::Packet& operator>>(sf::Packet& packet, SurrenderPacket& content);

sf::Packet& operator<<(sf::Packet& packet, const QuitLobbyPacket& content);
sf::Packet& operator>>(sf::Packet& packet, QuitLobbyPacket& content);