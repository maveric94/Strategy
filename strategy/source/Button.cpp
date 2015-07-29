#include "Button.h"

#include <string>

Button::Button(State::Context context,Buttons::ID id,const std::string name,sf::Vector2f position)
	: mName(sf::String(name),context.fonts->get(Fonts::ID::Main),20)
{
	mBody.setSize(sf::Vector2f(context.window->getSize() / 8u));
	mBody.setFillColor(sf::Color::Red);
	mBody.setOutlineColor(sf::Color::Green);
	mBody.setOutlineThickness(5.f);
	mName.setColor(sf::Color::Black);
	mName.setStyle(sf::Text::Bold);
	this->id = id;
	this->setPosition(position);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mBody,states);
	target.draw(mName,states);
}

bool Button::isPressed(sf::Vector2f cursorPos)
{
	return getTransform().transformRect(mBody.getGlobalBounds()).contains(cursorPos);
}

Buttons::ID Button::getButtonID()
{
	return id;
}

sf::FloatRect Button::getBoundingRect()
{	
	return mBody.getGlobalBounds();
}
