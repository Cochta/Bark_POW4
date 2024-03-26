#include "packet.h"

Packet* Packet::FromType(PacketType type) {
  switch (type) {
    case PacketType::Connect:
      return new ConnectPacket();
    case PacketType::StartGame:
      return new StartGamePacket();
    case PacketType::HasPlayed:
      return new HasPlayedPacket();
    case PacketType::GameFinished:
      return new GameFinishedPacket();
    case PacketType::Surrender:
      return new SurrenderPacket();
    case PacketType::QuitLobby:
      return new QuitLobbyPacket();
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
    case PacketType::GameFinished: {
      const auto& gameFinishedPacket =
          dynamic_cast<const GameFinishedPacket&>(packetType);
      packet << gameFinishedPacket;
      break;
    }
    case PacketType::Surrender: {
      const auto& surrenderPacket =
          dynamic_cast<const SurrenderPacket&>(packetType);
      packet << surrenderPacket;
      break;
    }
    case PacketType::QuitLobby: {
      const auto& quitPacket = dynamic_cast<const QuitLobbyPacket&>(packetType);
      packet << quitPacket;
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
    case PacketType::GameFinished: {
      auto* gameFinishedPacket = dynamic_cast<GameFinishedPacket*>(&packetType);
      packet >> *gameFinishedPacket;
      break;
    }
    case PacketType::Surrender: {
      auto* surrenderPacket = dynamic_cast<SurrenderPacket*>(&packetType);
      packet >> *surrenderPacket;
      break;
    }
    case PacketType::QuitLobby: {
      auto* quitPacket = dynamic_cast<QuitLobbyPacket*>(&packetType);
      packet >> *quitPacket;
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
  return packet << content.p1Name << content.p1Elo << content.p2Name
                << content.p2Elo;
}

sf::Packet& operator>>(sf::Packet& packet, StartGamePacket& content) {
  packet >> content.p1Name >> content.p1Elo >> content.p2Name >>
      content.p2Elo;
  return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const HasPlayedPacket& content) {
  return packet << content.IsFirstTurn << content.X << content.Y
                << content.index;
}

sf::Packet& operator>>(sf::Packet& packet, HasPlayedPacket& content) {
  packet >> content.IsFirstTurn >> content.X >> content.Y >> content.index;
  return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const GameFinishedPacket& content) {
  return packet << content.HasP1Won;
}

sf::Packet& operator>>(sf::Packet& packet, GameFinishedPacket& content) {
  packet >> content.HasP1Won;
  return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const SurrenderPacket& content) {
  return packet << content.hasSurrendered;
}

sf::Packet& operator>>(sf::Packet& packet, SurrenderPacket& content) {
  packet >> content.hasSurrendered;
  return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const QuitLobbyPacket& content) {
  return packet << content.hasQuit;
}

sf::Packet& operator>>(sf::Packet& packet, QuitLobbyPacket& content) {
  packet >> content.hasQuit;
  return packet;
}
