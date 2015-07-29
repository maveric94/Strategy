#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState : public State
{
public:

	TitleState(StateStack& statestack,Context context);

	virtual void State::draw();
	virtual bool State::update(sf::Time dt);
	virtual bool State::handleEvent(sf::Event event);

private:

	sf::Text mText;
	sf::Sprite mSprite;
};

#endif 