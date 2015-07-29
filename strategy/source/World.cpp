#include "World.h"

#include <algorithm>

typedef std::tr1::shared_ptr<Unit> UnitPtr;

World::World(sf::RenderWindow& window , sf::TcpSocket& socket , TextureHolder* textureHolder)
	: mWindow(window)
	, mScroolSpeed(100.f)
	, mView(window.getDefaultView())
	, mMiniMapView(window.getDefaultView())
	, mViewMovementDirection(0,0)
	, mSocket(socket)
	, mTextureHolder(textureHolder)
	, mMap(textureHolder->get(Textures::ID::MapTileset))
	, mWorldBounds(2000 , 2000)
{
	mMiniMapView.setViewport(sf::FloatRect(0.75f , 0 , 0.25f , 0.25f));
	registerUnit<BasicUnit>(Units::ID::BasicUnit);
	//registerUnit<BaseBuilding>(Units::ID::BaseBuilding , mTextureHolder->get(Textures::ID::MotherShip));

	mPlayerColors.push_back(sf::Color::Red);
	mPlayerColors.push_back(sf::Color::Blue);
	mPlayerColors.push_back(sf::Color::Green);
	mPlayerColors.push_back(sf::Color::Yellow);
	mPlayerColors.push_back(sf::Color::Magenta);
	mPlayerColors.push_back(sf::Color::Cyan);
	mPlayerColors.push_back(sf::Color::White);
	mPlayerColors.push_back(sf::Color::Black);

	mPlayerSpawnPoints.push_back(sf::Vector2f(10 , 10));
	mPlayerSpawnPoints.push_back(sf::Vector2f(100 , 10));
	mPlayerSpawnPoints.push_back(sf::Vector2f(10 , 100));
	mPlayerSpawnPoints.push_back(sf::Vector2f(100 , 100));
}

void World::draw()
{
	mWindow.setView(mView);
	drawDetails();
	mMiniMapView.setCenter(mView.getCenter());
	mWindow.setView(mMiniMapView);
	drawDetails();
	mWindow.setView(mView);
}

void World::drawDetails()
{
	mWindow.draw(mMap);

	for(auto itr = mUnits.begin();itr != mUnits.end(); ++itr)
		for(auto itr1 = itr->begin(); itr1 != itr->end(); ++itr1)
			mWindow.draw(**itr1);

	for(auto itr = mProjectiles.begin(); itr != mProjectiles.end(); ++itr)
		mWindow.draw(*itr);
}

void World::update(sf::Time dt)
{
	mView.move(sf::Vector2f(mViewMovementDirection) * dt.asSeconds() * mScroolSpeed);

	//checkCollisions();

	//reactToEnemies();

	updateUnits(dt);
	updateProjectiles(dt);
	handleDestroyedUnits();
}

UnitPtr World::checkPointAlly(sf::Vector2f point,int& number)
{
	number = 0;
	for(auto itr = mUnits[mLocalPlayerID].begin(); itr != mUnits[mLocalPlayerID].end(); ++itr, number++)
		if((*itr)->contains(point))
			return *itr;
	number = 0;
	return nullptr;
}

UnitPtr World::checkPointEnemy(sf::Vector2f point,int& number, int& playerNumber)
{
	playerNumber = 0;
	number = 0;
	for(auto itr = mUnits.begin(); itr != mUnits.end(); ++itr , ++playerNumber)
	{
		if(itr == mUnits.begin() + mLocalPlayerID)
			continue;
		number = 0;
		for(auto itr1 = itr->begin(); itr1 != itr->end(); ++itr1, ++number)
			if((*itr1)->contains(point))
				return *itr1;
	}
	number = 0;
	playerNumber = 0;
	return nullptr;
}

void World::changeViewDirection(sf::Vector2i offset)
{
	mViewMovementDirection += offset;
}

void World::createUnit(sf::Vector2f position,Units::ID unitID)
{
	UnitPtr newUnit = mFactories.find(unitID)->second(mPlayerColors[mLocalPlayerID]);
	newUnit->setPosition(position);
	mUnits[mLocalPlayerID].push_back(newUnit);
}

sf::Vector2f World::transformCoord(sf::Vector2i pos)
{
	return mWindow.mapPixelToCoords(pos,mView);
}

void World::addEnemies(int number)
{
	for(int i = 0; i < number + 1; i++)
	{
		//UnitPtr newBase = mFactories.find(Units::ID::BaseBuilding)->second(sf::Color::Black);
		//newBase->setPosition(mPlayerSpawnPoints[mUnits.size()]);
		std::vector<UnitPtr> newEnemy;
		//newEnemy.push_back(newBase);
		mUnits.push_back(newEnemy);
	}
}

void World::createEnemyUnit(sf::Vector2f position,Units::ID unitID, int enemynumber)
{
	UnitPtr newUnit = mFactories[unitID](mPlayerColors[enemynumber]);
	newUnit->setPosition(position);
	mUnits[enemynumber].push_back(newUnit);
}

void World::performUnitMovement(int enemyNumber, int unitNumber, sf::Vector2f pos)
{
	UnitPtr unit = mUnits.at(enemyNumber).at(unitNumber);
	unit->setAction(Unit::Action::Movement);
	unit->setDestinationPoint(pos);
}

void World::setLocalPlayerID(int id)
{
	mLocalPlayerID = id;
}

void World::performUnitAttack(int player1 , int unit1 , int player2 , int unit2)
{
	UnitPtr attacker = mUnits.at(player1).at(unit1);
	attacker->setAction(Unit::Action::Attack);
	attacker->setTarget(mUnits.at(player2).at(unit2));
}

int World::getLocalPlayerID()
{
	return mLocalPlayerID;
}

void World::checkCollisions()
{
	unsigned int size1 = mUnits.size();
	unsigned int size3 = mProjectiles.size();
	
	for(unsigned int i = 0; i < size1; i++)
	{
		unsigned int size2 = mUnits[i].size();

		for(unsigned int j = 0; j < size2; j++)
		{
			for(unsigned int k = 0; k < size3; k++)
			{
				if(mUnits[i][j]->intersects(mProjectiles[k].getBounds()))
				{
					mUnits[i][j]->damage(20);
					mProjectiles[k].disableProjectile();
				}
			}
		}
	}
}

void World::reactToEnemies()
{
	unsigned int size1 = mUnits[mLocalPlayerID].size();
	for(unsigned int i = 0; i < size1; ++i)
	{
		unsigned int size2 = mUnits.size();
		for(unsigned int j = 0; j < size2; ++j)
		{
			if(j == mLocalPlayerID)
				continue;

			unsigned int size3 = mUnits[j].size();

			for(unsigned int k = 0; k < size3; ++k)
			{
				if(mUnits[mLocalPlayerID][i]->isDestroyed())
					break;
				if(mUnits[j][k]->isDestroyed())
					continue;
				sf::Vector2f vec(mUnits[mLocalPlayerID][i]->getPosition() - mUnits[j][k]->getPosition());
				float distance = std::sqrt(std::pow(vec.x , 2) + std::pow(vec.y , 2));

				if(distance <= mUnits[mLocalPlayerID][i]->getRange() 
					&& mUnits[mLocalPlayerID][i]->getAction() == Unit::Action::None 
					&& mUnits[mLocalPlayerID][i]->getTarget() == nullptr)
				{
					mUnits[mLocalPlayerID][i]->setAction(Unit::Action::Attack);
					mUnits[mLocalPlayerID][i]->setTarget(mUnits[j][k]);
					sf::Packet packet;
					packet << static_cast<sf::Int32>(Client::UnitAction);
					packet << static_cast<sf::Int32>(Unit::Action::Attack);
					packet << static_cast<sf::Int32>(i);
					packet << static_cast<sf::Int32>(j);
					packet << static_cast<sf::Int32>(k);
					mSocket.send(packet);
				}
			}
		}
	}
}

void World::updateUnits(sf::Time dt)
{
	unsigned int size = mUnits.size();

	for(unsigned int i = 0; i < size; i++)
	{
		for(unsigned int j = 0; j < mUnits[i].size(); j++)
		{
			if(mUnits[i][j]->isDestroyed())
			{
				mUnits[i][j]->setTarget(nullptr);
			}
			else
			{
				mUnits[i][j]->update(dt , mProjectiles);
			}
		}
	}
}

void World::updateProjectiles(sf::Time dt)
{
	for(unsigned int i = 0; i < mProjectiles.size(); i++)
	{
		if(mProjectiles[i].getState())
		{
			mProjectiles[i].update(dt);
		}
		else
		{
			std::iter_swap(mProjectiles.begin() + i , mProjectiles.end() - 1);
			mProjectiles.pop_back();
		}
	}
}

void World::handleDestroyedUnits()
{
	for(unsigned int i = 0; i < mUnits[mLocalPlayerID].size(); ++i)
	{
		if(mUnits[mLocalPlayerID][i]->isDestroyed())
		{
			sf::Packet packet;
			packet << static_cast<sf::Int32>(Client::PacketType::DestroyUnit);
			packet << static_cast<sf::Int32>(mLocalPlayerID);
			packet << static_cast<sf::Int32>(i);
			mSocket.send(packet);
			deleteUnit(mLocalPlayerID , i);
		}
	}
}

void World::deleteUnit(int playerNumber , int unitNumber)
{
	mUnits[playerNumber][unitNumber]->damage(100);
	mUnits[playerNumber][unitNumber]->setTarget(nullptr);
	mUnits[playerNumber].erase(mUnits[playerNumber].begin() + unitNumber);
	//std::iter_swap(mUnits[playerNumber].begin() + unitNumber , mUnits[playerNumber].end() - 1);
	//mUnits[playerNumber].pop_back();
}