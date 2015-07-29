#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "NetworkPackets.h"
#include "Unit.h"

#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\TcpListener.hpp>
#include <SFML\System\Thread.hpp>
#include <SFML\Network\Packet.hpp>
#include <SFML\System\Sleep.hpp>

class GameServer
{
public:

	struct RemotePeer
	{
		RemotePeer();

		sf::TcpSocket socket;
		int identifier;
		bool isReady;
	};

	GameServer();
	~GameServer();

	void executionThread();
	void setListening(bool newState);
	void handleIncomingConnections();
	void handleIncomingPackets();
	void handlePacket(sf::Int32 packetType,sf::Packet packet,int sender);
	void sendToAll(sf::Packet packet);
	void sendToAllExceptOne(sf::Packet packet,int id);
	
private:

	bool mListening;
	bool mThreadEnd;

	sf::Thread mThread;
	std::vector<RemotePeer*> mPeers;
	sf::TcpListener mListener;
	std::size_t mMaxConnectedPlayers;
	std::size_t mConnectedPlayers;
};

#endif 