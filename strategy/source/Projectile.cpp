#include "Projectile.h"
#include "Unit.h"

Projectile::Projectile(sf::Vector2f position , sf::Color color , UnitPtr target)
	: mBody(sf::Vector2f(10,10))
	, mIsActive(true)
	, mCurrentLifetime(sf::microseconds(0))
	, mLifetime(sf::seconds(500000))
	, mSpeed(150.f)
	, mTarget(target)
{
	mBody.setFillColor(color);
	setPosition(position);
	setOrigin(5.f , 5.f);
}
void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mBody , states);
}

void Projectile::update(sf::Time dt)
{
	if(mTarget->isDestroyed())
	{
		mIsActive = false;
		mTarget = nullptr;
		return;
	}
	mCurrentLifetime += dt;
	if(mCurrentLifetime > mLifetime)
	{
		mIsActive = false;
		return;
	}

	sf::Vector2f dir(mTarget->getPosition() - getPosition());
	if(mTarget->intersects(getBounds()))
	{
		mTarget->damage(20);
		mIsActive = false;
		return;
	}
	float abs = std::sqrt(std::pow(dir.x, 2) + std::pow(dir.y, 2));
	float factor = mSpeed * dt.asSeconds();
	move(sf::Vector2f(dir) / abs * factor);
}

void Projectile::disableProjectile()
{
	mIsActive = false;
}

bool Projectile::getState()
{
	return mIsActive;
}

sf::FloatRect Projectile::getBounds()
{
	return getTransform().transformRect(mBody.getLocalBounds());
}
