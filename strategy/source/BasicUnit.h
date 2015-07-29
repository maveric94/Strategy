#ifndef BASICUNIT_H
#define BASICUNIT_H

#include "Unit.h"


class BasicUnit : public Unit
{
public:

	BasicUnit();

	virtual void attack(std::vector<Projectile>& projectiles , sf::Time dt);
	virtual void initializeShape(sf::Color color);
	virtual bool contains(sf::Vector2f point);
	virtual bool intersects(sf::FloatRect rect);
	virtual void draw(sf::RenderTarget& target , sf::RenderStates states) const;

private:

	sf::RectangleShape mBody;

};

#endif 