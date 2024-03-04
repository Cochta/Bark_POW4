#pragma once

#include <SFML/Network.hpp>

#include <queue>
#include <shared_mutex>
#include <mutex>

struct Client
{
	sf::TcpSocket* socket = new sf::TcpSocket();
	std::queue<sf::Packet*> packetsToBeSent = std::queue<sf::Packet*>();
	mutable std::shared_mutex mutex_;

	void SendPacket(sf::Packet* packet)
	{
		std::scoped_lock lock(mutex_);
		packetsToBeSent.push(packet);
	}

	bool IsPacketsEmpty() const
	{
		std::shared_lock lock(mutex_);
		return packetsToBeSent.empty();
	}
};
