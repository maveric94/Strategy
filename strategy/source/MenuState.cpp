#include "MenuState.h"


MenuState::MenuState(StateStack& stack,Context context)
	: State(stack,context)
{
	mButtonList.push_back(Button(context,Buttons::ID::HostGame,std::string("Host Game"),sf::Vector2f(500,100)));
	mButtonList.push_back(Button(context,Buttons::ID::JoinGame,std::string("Join Game"),sf::Vector2f(500,200)));
	//mButtonList.push_back(Button(context,Buttons::ID::Settings,std::string("Settings"),sf::Vector2f(500,300)));
	mButtonList.push_back(Button(context,Buttons::ID::Quit,std::string("Quit"),sf::Vector2f(500,400)));

}

bool MenuState::update(sf::Time dt)
{
	return false;
}

bool MenuState::handleEvent(sf::Event event)
{
	if(event.type == sf::Event::MouseButtonPressed)
	{
		for(auto itr = mButtonList.begin(); itr != mButtonList.end();itr++)
		{
			if (itr->isPressed(sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y)))
			{
				switch(itr->getButtonID())
				{
					case Buttons::ID::HostGame : 
						State::requestStackPush(States::ID::HostGame);
						break;

					case Buttons::ID::JoinGame : 
						State::requestStackPush(States::ID::JoinGame);
						break;

					case Buttons::ID::Quit : 
						State::requestStateClear();
						break;

					//case Buttons::ID::Settings : State::requestStackPush(States::ID::SettingsState);return false;
				}
			}
		}
	}
	return false;
}

void MenuState::draw()
{
	sf::RenderWindow& window =* getContext().window;
	window.clear();
	for(auto itr = mButtonList.begin(); itr != mButtonList.end(); itr++)
	{
		window.draw(*itr);
	}
}

