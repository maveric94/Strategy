#include "MultiplayerGameState.h"

#include <fstream>

sf::IpAddress getAddressFromFile()
{
	std::ifstream inputFile("ip.txt");
	std::string ipAddress;
	if (inputFile >> ipAddress)
		return ipAddress;

	std::ofstream outputFile("ip.txt");
	std::string localAddress = "127.0.0.1";
	outputFile << localAddress;
	return localAddress;
}

MultiplayerGameState::MultiplayerGameState(StateStack& stack,Context context,bool isHost)
	: State(stack,context)
	, mWorld(*context.window , mSocket , context.textures)
	, mPlayer(&mWorld,&mSocket)
	, mHost(isHost)
	, mConnected(false)
	, mGameStarted(false)
	, mNumber(0)
{
	sf::IpAddress ip;
	if(mHost)
	{
		mGameServer = new GameServer;
		ip = "127.0.0.1";
	}
	else
	{
		mGameServer = nullptr;
		ip = getAddressFromFile();
	}

	if(mSocket.connect(ip,55001,sf::seconds(10)) == sf::TcpSocket::Status::Done)
		mConnected = true;

	mSocket.setBlocking(false);

	sf::Text text(sf::String("Press Space to start the game") ,  getContext().fonts->get(Fonts::ID::Main));
	text.setPosition(100,100);
	text.setColor(sf::Color::White);
	mMessages.push_back(text);
	text.setString(sf::String("Waiting host to start the game"));
	mMessages.push_back(text);
	text.setPosition(100,150);
	text.setString(sf::String("Current players number:"));
	mMessages.push_back(text);
	text.setPosition(400,150);
	mMessages.push_back(text);
}

MultiplayerGameState::~MultiplayerGameState()
{
	delete mGameServer;
}

bool MultiplayerGameState::handleEvent(sf::Event event)
{
	if(mGameStarted)
		mPlayer.handleEvent(event);


	if(event.type == sf::Event::EventType::KeyPressed)
	{
		if(event.key.code == sf::Keyboard::Escape)
			State::requestStackPop();

		if(event.key.code == sf::Keyboard::Space && mHost && !mGameStarted)
		{
			sf::Packet packet;
			packet << static_cast<sf::Int32>(Client::PacketType::GameStart);
			mSocket.send(packet);
		}
	}

	return false;
}


void MultiplayerGameState::draw()
{
	getContext().window->clear();



	mWorld.draw(); 

	if(!mGameStarted)
	{
		if(mHost)
			getContext().window->draw(mMessages[0]);

		else
			getContext().window->draw(mMessages[1]);

		getContext().window->draw(mMessages[2]);
		char str[2];
		_itoa_s(mNumber + 1 , str , 10);
		mMessages[3].setString(sf::String(str));
		getContext().window->draw(mMessages[3]);

	}

}

bool MultiplayerGameState::update(sf::Time dt)
{
	if(!mConnected)
	{
		State::requestStackPop();
		return false;
	}
	else
	{
		if(mGameStarted)
			mWorld.update(dt);

		sf::Packet packet;
		if(mSocket.receive(packet) == sf::Socket::Status::Done)
		{
			sf::Int32 packetType;
			packet >> packetType;
			handlePacket(packet,packetType);
		}

	}

	return false;
}

void MultiplayerGameState::handlePacket(sf::Packet packet,sf::Int32 packetType)
{
	sf::Vector2f pos;
	sf::Int32 playerNumber;
	sf::Int32 unitID;
	sf::Int32 actionID;
	sf::Int32 unitNumber;
	sf::Int32 targetPlayerNumber;
	sf::Int32 targetUnitNumber;

	switch(packetType)
	{
		case Server::PacketType::InitData :

			packet >> playerNumber;
			mWorld.addEnemies(playerNumber);
			mWorld.setLocalPlayerID(playerNumber);
			mNumber = playerNumber;
			break;

		case Server::PacketType::AddPlayer :

			mWorld.addEnemies(0);
			mNumber++;
			break;

		case Server::PacketType::GameStart :

			mGameStarted = true;
			break;

		case Server::PacketType::CreateUnit :

			packet >> playerNumber;
			packet >> unitID;
			packet >> pos.x;
			packet >> pos.y;

			mWorld.createEnemyUnit(pos,static_cast<Units::ID>(unitID),playerNumber);
			break;

		case Server::PacketType::UnitAction :

			packet >> playerNumber;
			packet >> unitNumber;
			packet >> actionID;

			switch(actionID)
			{
				case Unit::Action::Movement :

					packet >> pos.x;
					packet >> pos.y;
					mWorld.performUnitMovement(playerNumber,unitNumber,pos);
					break;

				case Unit::Action::Attack :

					packet >> targetPlayerNumber;
					packet >> targetUnitNumber;

					mWorld.performUnitAttack(playerNumber , unitNumber , targetPlayerNumber , targetUnitNumber);

					break;
			}
			break;

		case Server::PacketType::DestroyUnit :

			packet >> playerNumber;
			packet >> unitNumber;
			mWorld.deleteUnit(playerNumber , unitNumber);
			break;

	}

}