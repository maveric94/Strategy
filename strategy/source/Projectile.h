#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\System\Time.hpp>

#include <memory>

class Unit;

typedef std::shared_ptr<Unit> UnitPtr;

class Projectile : public sf::Drawable , public sf::Transformable
{
public:

	Projectile(sf::Vector2f position , sf::Color color , UnitPtr target);
	
	void update(sf::Time dt);
	void disableProjectile();
	bool getState();
	sf::FloatRect getBounds();

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	bool mIsActive;
	float mSpeed;

	UnitPtr mTarget;

	sf::RectangleShape mBody;
	sf::Time mCurrentLifetime;
	sf::Time mLifetime;
};

#endif 