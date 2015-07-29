#include "GameServer.h"

GameServer::GameServer()
	: mConnectedPlayers(0)
	, mMaxConnectedPlayers(10)
	, mThread(&GameServer::executionThread,this)
	, mThreadEnd(false)
{
	mThread.launch();
	mListener.setBlocking(false);
}

GameServer::~GameServer()
{
	mThreadEnd = true;
	mThread.wait();
}
GameServer::RemotePeer::RemotePeer()
	: isReady(false)
{
	socket.setBlocking(false);
}

void GameServer::executionThread()
{
	setListening(true);

	while(!mThreadEnd)
	{
		if(mConnectedPlayers >= mMaxConnectedPlayers)
		{
			setListening(false);
		}
		
		handleIncomingConnections();
		handleIncomingPackets();

		sf::sleep(sf::milliseconds(10));
		
	}
}

void GameServer::setListening(bool newstate)
{
	if(newstate)
	{
		mListening = (mListener.listen(55001) == sf::TcpListener::Status::Done);
	}
	else
	{
		mListening = false;
		mListener.close();
	}
}

void GameServer::handleIncomingConnections()
{
	if(mListening)
	{
		RemotePeer* newPeer = new RemotePeer;
		if(mListener.accept(newPeer->socket) == sf::TcpListener::Status::Done)
		{
			sf::Packet packet;
			packet << static_cast<sf::Int32>(Server::PacketType::InitData);
			packet << static_cast<sf::Int32>(mPeers.size());
			newPeer->socket.send(packet);
			newPeer->isReady = true;
			newPeer->identifier = mPeers.size();
			packet.clear();
			packet << static_cast<sf::Int32>(Server::PacketType::AddPlayer);
			sendToAllExceptOne(packet , mPeers.size());
			mPeers.push_back(newPeer);
			mConnectedPlayers++;

		}
		else
		{
			delete newPeer;
		}
	}
}

void GameServer::handleIncomingPackets()
{
	for(auto itr = mPeers.begin(); itr != mPeers.end(); ++itr)
	{
		sf::Packet packet;
		if((*itr)->socket.receive(packet) == sf::Socket::Status::Done)
		{
			sf::Int32 packetType;
			packet >> packetType;
			handlePacket(packetType,packet,(*itr)->identifier);
		}

	}
}

void GameServer::sendToAll(sf::Packet packet)
{
	for(auto itr = mPeers.begin(); itr != mPeers.end(); ++itr)
	{
		if((*itr)->isReady)
		{
			(*itr)->socket.send(packet);
		}
	}
}

void GameServer::handlePacket(sf::Int32 packetType,sf::Packet packet,int sender)
{
	switch(packetType)
	{
		case Client::PacketType::GameStart :

			{
			
				packet.clear();
				packet << static_cast<sf::Int32>(Server::PacketType::GameStart);
				setListening(false);
				sendToAll(packet);
				break;
			}

		case Client::PacketType::CreateUnit :

			{
				float x,y;
				sf::Int32 unitID;
				packet >> unitID;
				packet >> x;
				packet >> y;
				packet.clear();
				packet << static_cast<sf::Int32>(Server::PacketType::CreateUnit);
				packet << static_cast<sf::Int32>(sender);
				packet << unitID;
				packet << x;
				packet << y;
				sendToAllExceptOne(packet,sender);
				break;
			}

		case Client::PacketType::DestroyUnit :

			{
				sf::Int32 unitNumber;
				sf::Int32 playerNumber;

				packet >> playerNumber;
				packet >> unitNumber;
				packet.clear();
				packet << static_cast<sf::Int32>(Server::PacketType::DestroyUnit);
				packet << static_cast<sf::Int32>(sender);
				packet << unitNumber;
				sendToAllExceptOne(packet , sender);
				break;

			}

		case Client::PacketType::Quit :

			break;

		case Client::PacketType::UnitAction :

			{

				sf::Int32 actionID;
				sf::Int32 unitNumber;
	
				packet >> actionID;
				packet >> unitNumber;

				switch(actionID)
				{
					case Unit::Action::Movement :

						{

							float x,y;

							packet >> x;
							packet >> y;
							packet.clear();
							packet << static_cast<sf::Int32>(Server::PacketType::UnitAction);
							packet << static_cast<sf::Int32>(sender);
							packet << unitNumber;
							packet << actionID;
							packet << x;
							packet << y;
							break;
						}

				case Unit::Action::Attack :

					{

						sf::Int32 targetPlayerNumber;
						sf::Int32 targetUnitNumber;

						packet >> targetPlayerNumber;
						packet >> targetUnitNumber;

						packet.clear();
	
						//if(sender <= targetPlayerNumber)
							//targetPlayerNumber++;

						packet << static_cast<sf::Int32>(Server::PacketType::UnitAction);
						packet << static_cast<sf::Int32>(sender);
						packet << unitNumber;
						packet << actionID;
						packet << targetPlayerNumber;
						packet << targetUnitNumber;
						break;
					}
				}

				sendToAllExceptOne(packet,sender);
				break;

			}
			break;

	}
}

void GameServer::sendToAllExceptOne(sf::Packet packet,int number)
{
	for(auto itr = mPeers.begin(); itr != mPeers.end(); ++itr)
	{
		if((*itr)->identifier == number)
			continue;
		else
			(*itr)->socket.send(packet);
	}
}
