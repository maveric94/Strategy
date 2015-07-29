#include "BasicUnit.h"

BasicUnit::BasicUnit()
	: Unit(100, 100, 200, 10, sf::seconds(2.f))
	, mBody(sf::Vector2f(20.f,20.f))
{
}

void BasicUnit::attack(std::vector<Projectile>& projectiles , sf::Time dt)
{
	sf::Vector2f vec(getPosition() - mTarget->getPosition());
	float distance = std::sqrt(std::pow(vec.x , 2) + std::pow(vec.y , 2));

	if(distance > getRange())
	{
		setIsMovingToAttack(true);
	}
	else
	{
		setIsMovingToAttack(false);
	}

	if(getIsMovingToAttack())
	{
		moveUnit(dt);
	}

	if(mTarget->isDestroyed())
	{
		mTarget = nullptr;
		setAction(Action::None);
		return;
	}
			
			
	if(getCurrentDelay() <= sf::Time::Zero && !getIsMovingToAttack())
	{
		setCurrentDelay(getAttackDelay());

		sf::Vector2f dir(mTarget->getPosition() - getPosition());
		float abs = std::sqrt(std::pow(dir.x, 2) + std::pow(dir.y, 2));
		sf::Vector2f vec(dir / abs);
		projectiles.push_back(Projectile(getPosition() + (vec * 25.f) , mBody.getFillColor() , mTarget)); 
			
	}
	
}

void BasicUnit::initializeShape(sf::Color color)
{
	mBody.setFillColor(color);
	mBody.setOrigin(10.f,10.f);
}

void BasicUnit::draw(sf::RenderTarget& target , sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mBody,states);
}

bool BasicUnit::contains(sf::Vector2f point)
{
	return getTransform().transformRect(mBody.getGlobalBounds()).contains(point);
}

bool BasicUnit::intersects(sf::FloatRect rect)
{
	return getTransform().transformRect(mBody.getGlobalBounds()).intersects(rect);
}

