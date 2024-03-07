#include "packet.h"

Packet* Packet::FromType(PacketType type) {
  switch (type) {
    case PacketType::Connect:
      return new ConnectPacket();
    case PacketType::StartGame:
      return new StartGamePacket();
    case PacketType::HasPlayed:
      return new HasPlayedPacket();
    default:
      return new InvalidPacket();
  }
}

sf::Packet& operator<<(sf::Packet& packet, const Packet& packetType) {
  packet << static_cast<sf::Uint8>(packetType.type);

  switch (packetType.type) {
    case PacketType::Connect: {
      const auto& connectPacket =
          dynamic_cast<const ConnectPacket&>(packetType);
      packet << connectPacket;
      break;
    }
    case PacketType::StartGame: {
      const auto& startGamePacket =
          dynamic_cast<const StartGamePacket&>(packetType);
      packet << startGamePacket;
      break;
    }
    case PacketType::HasPlayed: {
      const auto& hasPlayedPacket =
          dynamic_cast<const HasPlayedPacket&>(packetType);
      packet << hasPlayedPacket;
      break;
    }
    default:
      break;
  }

  return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Packet& packetType) {
  switch (packetType.type) {
    case PacketType::Connect: {
      auto* connectPacket = dynamic_cast<ConnectPacket*>(&packetType);
      packet >> *connectPacket;
      break;
    }
    case PacketType::StartGame: {
      auto* startGamePacket = dynamic_cast<StartGamePacket*>(&packetType);
      packet >> *startGamePacket;
      break;
    }
    case PacketType::HasPlayed: {
      auto* hasPlayedPacket = dynamic_cast<HasPlayedPacket*>(&packetType);
      packet >> *hasPlayedPacket;
      break;
    }
    default:
      break;
  }

  return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const ConnectPacket& message) {
  return packet << message.playerName;
}

sf::Packet& operator>>(sf::Packet& packet, ConnectPacket& message) {
  packet >> message.playerName;
  return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const StartGamePacket& content) {
  return packet << content.start;
}

sf::Packet& operator>>(sf::Packet& packet, StartGamePacket& content) {
  packet >> content.start;
  return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const HasPlayedPacket& content) {
  return packet << content.IsFirstTurn << content.X << content.Y << content.index;
}

sf::Packet& operator>>(sf::Packet& packet, HasPlayedPacket& content) {
  packet >> content.IsFirstTurn >> content.X >> content.Y >> content.index;
  return packet;
}
