#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML\Window\Event.hpp>

#include "StateStack.h"
#include "ResourceHolder.h"
#include "ResourceID.h"



class Application
{
public:
	Application();

	void run();

private:
	void processInput();
	void update(sf::Time dt);
	void render();
	void registerStates();


	TextureHolder mTextures;
    FontHolder mFonts;

	sf::RenderWindow mWindow;
	StateStack mStateStack;

};

#endif 