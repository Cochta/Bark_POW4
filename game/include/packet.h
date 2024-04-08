#pragma once

#include <SFML/Network.hpp>

#include "Metrics.h"

/**
 * @brief Enumeration of different types of packets.
 */
enum class PacketType {
  Connect,
  StartGame,
  HasPlayed,
  GameFinished,
  Surrender,
  QuitLobby,
  Invalid
};
/**
 * @brief Base class for packet structures.
 */
struct Packet {
  Packet() = default;
  explicit Packet(PacketType type) : type(type) {}
  virtual ~Packet() = default;

  PacketType type = PacketType::Invalid;

  static Packet* FromType(PacketType type);
};
/**
 * @brief Packet structure for establishing a connection.
 */
struct ConnectPacket final : Packet {
  ConnectPacket() : Packet(PacketType::Connect) {}
  explicit ConnectPacket(std::string_view playerName)
      : Packet(PacketType::Connect), playerName(playerName) {}

  std::string playerName;
};

/**
 * @brief Packet structure for starting a game.
 */
struct StartGamePacket final : Packet {
  StartGamePacket() : Packet(PacketType::StartGame) {}
  std::string p1Name;
  int p1Elo;
  std::string p2Name;
  int p2Elo;
};

/**
 * @brief Packet structure for indicating a player's move.
 */
struct HasPlayedPacket final : Packet {
  HasPlayedPacket() : Packet(PacketType::HasPlayed) {}
  bool IsFirstTurn = true;
  float X = 0, Y = 0;
  int index = 0;
};

/**
 * @brief Packet structure for indicating the end of a game.
 */
struct GameFinishedPacket final : Packet {
  GameFinishedPacket() : Packet(PacketType::GameFinished) {}
  bool HasP1Won = true;
};

/**
 * @brief Packet structure for indicating a player's surrender.
 */
struct SurrenderPacket final : Packet {
  SurrenderPacket() : Packet(PacketType::Surrender) {}
  bool hasSurrendered = true;
};

/**
 * @brief Packet structure for indicating a player's quitting the lobby.
 */
struct QuitLobbyPacket final : Packet {
  QuitLobbyPacket() : Packet(PacketType::QuitLobby) {}
  bool hasQuit = true;
};

/**
 * @brief Packet structure for indicating a packet is invalid
 */
struct InvalidPacket final : Packet {
  InvalidPacket() : Packet(PacketType::Invalid) {}
};

// Operator overloads for serialization

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