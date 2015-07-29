#ifndef STATESTACK_H
#define STATESTACK_H

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include "State.h"

#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
public:
	
	enum Action
	{
		Pop,
		Push,
		Clear,
	};


	StateStack(State::Context context);

	template <typename T>
	void registerState(States::ID stateID);

	template <typename T, typename Param1>
	void registerState(States::ID stateID, Param1 arg1);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(States::ID stateID);
	void popState();
	void clearStates();
		
	bool isEmpty() const;

private:

	struct PendingChange
	{
		PendingChange(Action action , States::ID state = States::ID::None);

		Action action;
		States::ID state;
	};


	State::Ptr createState(States::ID stateID);
	void applyPendingChanges();

private:

	std::vector<State::Ptr> mStack;
	std::map<States::ID, std::function<State::Ptr()>> mFactories;

	State::Context mContext;
	
	std::vector<PendingChange> mPendingList;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this] ()
	{
		return State::Ptr(new T(*this, mContext));
	};
}

template <typename T, typename Param1>
void StateStack::registerState(States::ID stateID, Param1 arg1)
{
	mFactories[stateID] = [this, arg1] ()
	{
		return State::Ptr(new T(*this, mContext, arg1));
	};
}

#endif
