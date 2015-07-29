#ifndef PLAYER_H
#define PLAYER_H

#include "World.h"
#include "NetworkPackets.h"

#include <SFML\Window\Event.hpp>
#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\Packet.hpp>

class Player
{
public:

	Player(World* world,sf::TcpSocket* socket);
	~Player();

	void handleEvent(sf::Event event);

private:

	void handleKeyboardInput(sf::Keyboard::Key key,int isPressed);
	void handleMouseInput(sf::Event::MouseButtonEvent mouse);

private:

	Units::ID mUnitOnCreate;
	UnitPtr mSelectedUnit;
	World* mWorld;
	sf::TcpSocket* mSocket;
};

#endif