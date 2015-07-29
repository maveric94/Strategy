#ifndef STATE_H
#define STATE_H

#include<SFML\System\Time.hpp>
#include<SFML\Window\Event.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

#include "StatesID.h"
#include "ResourceID.h"
#include "ResourceHolder.h"

#include <memory>

class StateStack;

class State
{
public:

	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& window,TextureHolder& textures,FontHolder& fonts);

		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder*	fonts;
	};

	State(StateStack& stack,Context context);

	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(sf::Event event) = 0;

protected:
	void requestStackPush(States::ID state);
	void requestStackPop();
	void requestStateClear();

	Context getContext();

private:
	StateStack* mStack;
	Context mContext;
};

#endif 