#ifndef BASEBUILDING_H
#define BASEBUILDING_H

#include "Unit.h"

#include <SFML\Graphics\Texture.hpp>

class World;

#include <SFML\Graphics\Sprite.hpp>

class BaseBuilding : public Unit
{
public:

	BaseBuilding(World& world , sf::Texture texture);

	void Unit::attack(std::vector<Projectile>& projectiles , sf::Time dt);
	bool contains(sf::Vector2f point);
	bool intersects(sf::FloatRect rect);
	void initializeShape(sf::Color color);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	sf::Sprite mSprite;
	World& mWorld;
	sf::Texture mTexture;
};

#endif