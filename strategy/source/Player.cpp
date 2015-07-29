#include "Player.h"

Player::Player(World* world,sf::TcpSocket* socket)
	: mWorld(world)
	, mSelectedUnit(nullptr)
	, mUnitOnCreate(Units::ID::None)
	, mSocket(socket)
{
}

Player::~Player()
{
	mSelectedUnit = nullptr;
}

void Player::handleEvent(sf::Event event)
{
	if(event.type == sf::Event::EventType::KeyPressed)
		handleKeyboardInput(event.key.code,1);
	else if(event.type == sf::Event::EventType::KeyReleased)
		handleKeyboardInput(event.key.code,-1);
	else if(event.type == sf::Event::EventType::MouseButtonPressed)
		handleMouseInput(event.mouseButton);
	
}

void Player::handleKeyboardInput(sf::Keyboard::Key key,int isPressed)
{
	switch(key)
	{
		case sf::Keyboard::Key::Up : 

			mWorld->changeViewDirection(sf::Vector2i(0,-1) * isPressed);
			return;

		case sf::Keyboard::Key::Down :

			mWorld->changeViewDirection(sf::Vector2i(0,1) * isPressed);
			return;

		case sf::Keyboard::Key::Left :

			mWorld->changeViewDirection(sf::Vector2i(-1,0) * isPressed);
			return;

		case sf::Keyboard::Key::Right :

			mWorld->changeViewDirection(sf::Vector2i(1,0) * isPressed);
			return;

	}
	if(isPressed == 1)
	{
		if(key == sf::Keyboard::Key::Num1)
		{
			mUnitOnCreate = Units::ID::BasicUnit;
			mSelectedUnit = nullptr;
			return;
		}
	}
}

void Player::handleMouseInput(sf::Event::MouseButtonEvent mouse)
{
	static int number;
	int playerNumber;
	int unitNumber;

	if(mSelectedUnit && mSelectedUnit->isDestroyed())
		mSelectedUnit = nullptr;

	sf::Vector2f mousePos = mWorld->transformCoord(sf::Vector2i(mouse.x,mouse.y));
	if(mouse.button == sf::Mouse::Button::Left)
	{
		if(mUnitOnCreate == Units::ID::None)
		{
			mSelectedUnit = mWorld->checkPointAlly(mousePos,number);
		}
		else
		{
			mWorld->createUnit(mousePos,mUnitOnCreate);
			mUnitOnCreate = Units::ID::None;

			sf::Packet packet;
			packet << static_cast<sf::Int32>(Client::PacketType::CreateUnit);
			packet << static_cast<sf::Int32>(Units::ID::BasicUnit);
			packet << mousePos.x;
			packet << mousePos.y;
			mSocket->send(packet);
		}
	}
	else if(mouse.button == sf::Mouse::Button::Right && mSelectedUnit != nullptr)
	{
		UnitPtr newTarget = mWorld->checkPointEnemy(mousePos,unitNumber,playerNumber);
		mSelectedUnit->setTarget(newTarget);
		if(newTarget == nullptr)
		{
			sf::Packet packet;
			mSelectedUnit->setAction(Unit::Action::Movement);
			mSelectedUnit->setDestinationPoint(mousePos);
			packet << static_cast<sf::Int32>(Client::PacketType::UnitAction);
			packet << static_cast<sf::Int32>(Unit::Action::Movement);
			packet << static_cast<sf::Int32>(number);
			packet << mousePos.x;
			packet << mousePos.y;
			mSocket->send(packet);

		}
		else
		{
			sf::Packet packet;
			mSelectedUnit->setAction(Unit::Action::Attack);
			mSelectedUnit->setTarget(newTarget);

			packet << static_cast<sf::Int32>(Client::PacketType::UnitAction);
			packet << static_cast<sf::Int32>(Unit::Action::Attack);
			packet << static_cast<sf::Int32>(number);
			packet << static_cast<sf::Int32>(playerNumber);
			packet << static_cast<sf::Int32>(unitNumber);
			mSocket->send(packet);
		}
	}
}