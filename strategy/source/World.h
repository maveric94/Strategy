#ifndef WORLD_H
#define WORLD_H

#include <functional>
#include <map>

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\Packet.hpp>

#include "Map.h"
#include "Unit.h"
#include "UnitsID.h"
#include "BasicUnit.h"
#include "BaseBuilding.h"
#include "Projectile.h"
#include "NetworkPackets.h"
#include "ResourceHolder.h"
#include "ResourceID.h"

class World
{
public:

	World(sf::RenderWindow& window , sf::TcpSocket& socket , TextureHolder* holder);

	void update(sf::Time dt);
	void draw();

	void createUnit(sf::Vector2f position,Units::ID unitID);
	void createEnemyUnit(sf::Vector2f position,Units::ID unitID,int enemyNumber);
	void deleteUnit(int playerNumber , int unitNumber);

	UnitPtr checkPointAlly(sf::Vector2f point,int& number);
	UnitPtr checkPointEnemy(sf::Vector2f point,int& number , int& playerNumber);

	void changeViewDirection(sf::Vector2i offset);
	sf::Vector2f transformCoord(sf::Vector2i pos);
	void addEnemies(int number);

	void performUnitMovement(int enemyNumber, int unitNumber, sf::Vector2f pos);
	void performUnitAttack(int player1 , int unit1 , int player2 , int unit2);

	void setLocalPlayerID(int id);
	int getLocalPlayerID();

private:

	template <typename T>
	void registerUnit(Units::ID UnitID);

	template <typename T>
	void registerUnit(Units::ID unitID , sf::Texture& texture);

	void drawDetails();

	void checkCollisions();
	void reactToEnemies();
	void updateUnits(sf::Time dt);
	void updateProjectiles(sf::Time dt);
	void handleDestroyedUnits();

private:

	Map mMap;

	int mLocalPlayerID;
	float mScroolSpeed;

	sf::RenderWindow& mWindow;
	sf::View mView;
	sf::View mMiniMapView;
	sf::Vector2i mViewMovementDirection;
	sf::Vector2f mWorldBounds;

	std::vector<std::vector<UnitPtr>> mUnits;
	std::vector<Projectile> mProjectiles;
	std::map<Units::ID,std::function<UnitPtr(sf::Color)>> mFactories;

	std::vector<sf::Color> mPlayerColors;
	std::vector<sf::Vector2f> mPlayerSpawnPoints;

	sf::TcpSocket& mSocket;
	TextureHolder* mTextureHolder;
};

template <typename T>
void World::registerUnit(Units::ID unitID)
{
	mFactories[unitID] = [] (sf::Color color) -> UnitPtr
	{
		UnitPtr newUnit(dynamic_cast<Unit*>(new T));
		newUnit->initializeShape(color);
		return newUnit;
	};
}

template <typename T>
void World::registerUnit(Units::ID unitID , sf::Texture& texture)
{
	mFactories[unitID] = [texture , this] (sf::Color color) -> UnitPtr
	{
		UnitPtr newUnit(dynamic_cast<Unit*>(new T(*this , texture)));
		return newUnit;
	};
}

#endif