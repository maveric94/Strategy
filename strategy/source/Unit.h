#ifndef UNIT_H
#define UNIT_H

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\Graphics\VertexArray.hpp>

#include "Projectile.h"

#include <memory>

typedef std::shared_ptr<Unit> UnitPtr;

class Unit : public sf::Drawable , public sf::Transformable
{
public:

	enum Action 
	{
		None,
		Movement,
		Attack,
		UnitCreation,
	};

	Unit(int hitPoints, int speed, float range, int damage, sf::Time attackDelay);
	virtual ~Unit();

	void update(sf::Time dt, std::vector<Projectile>& projectiles);
	void setAttackedState(bool state);
	void setDestinationPoint(sf::Vector2f point);
	void setAction(Action action);
	Action getAction();
	void setTarget(UnitPtr target);
	UnitPtr getTarget();
	bool isDestroyed();
	void damage(int points);
	void repair(int points);
	void destroy();

	float getRange();
	void setRange(float range);

	bool getIsMovingToAttack();
	void setIsMovingToAttack(bool state);

	sf::Time getCurrentDelay();
	void setCurrentDelay(sf::Time delay);
	void updateCurrentDelay(sf::Time delay);

	sf::Time getAttackDelay();
	void setAttackDelay(sf::Time delay);

	virtual bool contains(sf::Vector2f point) = 0;
	virtual bool intersects(sf::FloatRect rect) = 0;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void attack(std::vector<Projectile>& projectiles , sf::Time dt) = 0;
	virtual void initializeShape(sf::Color color) = 0;

protected:

	void moveUnit(sf::Time dt);


protected:

	sf::RectangleShape mHitPointsBar;
	UnitPtr mTarget;

private:

	int mIdentifier;
	int mHitPoints;
	int mDamage;
	int mSpeed;
	bool mIsAttacked;
	sf::Time mAttackDelay;
	sf::Time mCurrentDelay;
	bool mMoveToAttack;
	float mRange;
	Action mCurrentAction;
	sf::Vector2f mDestinationPoint;
};

#endif