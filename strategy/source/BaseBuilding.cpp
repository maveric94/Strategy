#include "BaseBuilding.h"

#include "World.h"

BaseBuilding::BaseBuilding(World& world , sf::Texture texture)
	: Unit(1000 , 0 , 0 , 0 , sf::seconds(5))
	, mWorld(world)
	, mTexture(texture)
{
	setAction(Unit::Action::Attack);
	mSprite.setTexture(texture);
}

void BaseBuilding::draw(sf::RenderTarget& target , sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &mTexture;
	target.draw(mSprite , states);
}

void BaseBuilding::attack(std::vector<Projectile>& projectiles , sf::Time dt)
{
	if(getCurrentDelay() <= sf::Time::Zero)
	{
		setCurrentDelay(getAttackDelay());
		mWorld.createUnit(getPosition() + sf::Vector2f(20 , 20) , Units::ID::BasicUnit);
	}
}

void BaseBuilding::initializeShape(sf::Color color)
{
}

bool BaseBuilding::contains(sf::Vector2f point)
{
	return getTransform().transformRect(mSprite.getLocalBounds()).contains(point);
}

bool BaseBuilding::intersects(sf::FloatRect rect)
{
	return getTransform().transformRect(mSprite.getLocalBounds()).intersects(rect);
}

