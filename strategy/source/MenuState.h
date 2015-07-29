#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <vector>

#include "State.h"
#include "Button.h"

class MenuState : public State
{
public:

	MenuState(StateStack& stack,Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(sf::Event event);


private:

	std::vector<Button> mButtonList;
};

#endif 