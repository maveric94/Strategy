#ifndef MULTIPLAYERGAMESTATE_H
#define MULTIPLAYERGAMESTATE_H

#include "State.h"
#include "World.h"
#include "Player.h"
#include "GameServer.h"

#include <SFML\Network\IpAddress.hpp>
#include <SFML\Graphics\Text.hpp>

class MultiplayerGameState : public State
{
public:

	MultiplayerGameState(StateStack& stack,Context context,bool isHost);

	~MultiplayerGameState();

	void State::draw();
	bool State::handleEvent(sf::Event event);
	bool State::update(sf::Time dt);

private:

	void handlePacket(sf::Packet packet,sf::Int32 packetType);

private:

	bool mHost;
	bool mConnected;
	bool mGameStarted;

	int mNumber;

	GameServer* mGameServer;
	World mWorld;
	Player mPlayer;
	sf::TcpSocket mSocket;

	std::vector<sf::Text> mMessages;
};

#endif 