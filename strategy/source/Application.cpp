#include "Application.h"
#include "State.h"
#include "StatesID.h"
#include "StateStack.h"
#include "MenuState.h"
#include "TitleState.h"
#include "MultiplayerGameState.h"

#include <SFML\System\Time.hpp>

const sf::Time TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(800, 600), "Strategy", sf::Style::Close)
, mStateStack(State::Context::Context(mWindow,mTextures,mFonts))
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	mFonts.load(Fonts::ID::Main,"resources/times.TTF");

	mTextures.load(Textures::ID::MapTileset , "resources/images.png");
	mTextures.load(Textures::ID::MotherShip , "resources/Factory2.bmp");

	registerStates();

	mStateStack.pushState(States::ID::TitleState);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate(sf::Time::Zero);

	update(TimePerFrame);

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty())
				mWindow.close();
		}
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::EventType::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();
	mStateStack.draw();
	mWindow.setView(mWindow.getDefaultView());
	mWindow.display();
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::ID::TitleState);
	mStateStack.registerState<MenuState>(States::ID::MenuState);
	mStateStack.registerState<MultiplayerGameState>(States::ID::HostGame,true);
	mStateStack.registerState<MultiplayerGameState>(States::ID::JoinGame,false);
}

