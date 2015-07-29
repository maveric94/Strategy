#include "TitleState.h"

#include <SFML\Graphics\RectangleShape.hpp>

TitleState::TitleState(StateStack& statestack,Context context)
	: State(statestack,context)
	, mText(sf::String("Press Enter to start\nPress Escape to quit"),context.fonts->get(Fonts::ID::Main),25)
{
	mText.setPosition(sf::Vector2f(context.window->getSize() / 2u));
	mText.setOrigin(mText.getLocalBounds().width/2,mText.getLocalBounds().height/2);
	mText.setColor(sf::Color::White);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mText);
}

bool TitleState::handleEvent(sf::Event event)
{
	if(event.type == sf::Event::KeyPressed)
		switch(event.key.code)
		{
		case sf::Keyboard::Return : State::requestStackPush(States::ID::MenuState);break;
		case sf::Keyboard::Escape : State::requestStackPop();State::requestStackPush(States::ID::MenuState);break;
		}
	return false;
}

bool TitleState::update(sf::Time dt)
{
	return false;
}
