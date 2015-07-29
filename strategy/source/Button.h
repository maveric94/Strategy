#ifndef BUTTON_H
#define BUTTON_H

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

#include "State.h"
#include "ButtonID.h"
#include "ResourceHolder.h"
#include "ResourceID.h"

#include <string>

class Button : public sf::Drawable , public sf::Transformable
{
public:


	Button(State::Context context,Buttons::ID id,const std::string name,sf::Vector2f position);

	Buttons::ID getButtonID();
	bool isPressed(sf::Vector2f cursorPos);

	sf::FloatRect getBoundingRect();

	
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Buttons::ID id;
	sf::RectangleShape mBody;
	sf::Text mName;
};

#endif 