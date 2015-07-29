#include "Unit.h"

Unit::Unit(int hitPoints,int speed,float range,int damage,sf::Time attackDelay)
	: mHitPoints(hitPoints)
	, mSpeed(speed)
	, mRange(range)
	, mDamage(damage)
	, mAttackDelay(attackDelay)
	, mIsAttacked(false)
	, mCurrentAction(Action::None)
	, mCurrentDelay(sf::Time::Zero)
	, mTarget(NULL)
	, mDestinationPoint(0.f, 0.f)
	, mIdentifier(0)
	, mMoveToAttack(false)
{
	setRotation(0.f);
}

Unit::~Unit()
{

}

void Unit::update(sf::Time dt , std::vector<Projectile>& projectiles)
{
	mCurrentDelay -= dt;

	if(isDestroyed())
	{
		mTarget = nullptr;
		return;
	}


	switch(mCurrentAction)	
	{
			case None :
			{
					break;
			}
		

			case Movement :
			{
				moveUnit(dt);
				break;
			}
			

			case Attack :
			{
				attack(projectiles , dt);

				break;
			}

			
	}

}

void Unit::setAction(Unit::Action action)
{
	mCurrentAction = action;
}

void Unit::setDestinationPoint(sf::Vector2f point)
{
	mDestinationPoint = point;
}

void Unit::setTarget(UnitPtr target)
{
	mTarget = target;
}

void Unit::moveUnit(sf::Time dt)
{
	sf::Vector2f point;

	if(mCurrentAction == Action::Movement)
		point = mDestinationPoint;
	else
		point = mTarget->getPosition();

	sf::Vector2f pos = getPosition();
	sf::Vector2f dir(point - pos);
	if(std::abs(dir.x) < 1.f && std::abs(dir.y) < 1.f)
	{
		mCurrentAction = Action::None;
	}
	float abs = std::sqrt(std::pow(dir.x, 2) + std::pow(dir.y, 2));
	float factor = mSpeed * dt.asSeconds();
	move(sf::Vector2f(dir) / abs * factor);
}

void Unit::damage(int points)
{
	mHitPoints -= points;
}

bool Unit::isDestroyed()
{
	return mHitPoints <= 0;
}

void Unit::destroy()
{

}

float Unit::getRange()
{
	return mRange;
}

void Unit::setRange(float range)
{
	mRange = range;
}

Unit::Action Unit::getAction()
{
	return mCurrentAction;
}

UnitPtr Unit::getTarget()
{
	return mTarget;
}

bool Unit::getIsMovingToAttack()
{
	return mMoveToAttack;
}

void Unit::setIsMovingToAttack(bool state)
{
	mMoveToAttack = state;
}

void Unit::setCurrentDelay(sf::Time delay)
{
	mCurrentDelay = delay;
}

void Unit::updateCurrentDelay(sf::Time delay)
{
	mCurrentDelay += delay;
}

sf::Time Unit::getCurrentDelay()
{
	return mCurrentDelay;
}

sf::Time Unit::getAttackDelay()
{
	return mAttackDelay;
}

void Unit::setAttackDelay(sf::Time delay)
{
	mAttackDelay = delay;
}

