#include"StateStack.h"

#include <cassert>

StateStack::StateStack(State::Context context)
	: mStack()
	, mContext(context)
	, mFactories()
	, mPendingList()
{
}

StateStack::PendingChange::PendingChange(Action action , States::ID state)
	: action(action)
	, state(state)
{
}

void StateStack::update(sf::Time dt)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	for (auto itr = mStack.begin(); itr != mStack.end(); ++itr)
	{
		 (*itr)->draw();
	}
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Action::Push , stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	return found->second();
}

void StateStack::applyPendingChanges()
{
	for(auto itr = mPendingList.begin(); itr != mPendingList.end(); ++itr)
	{
		switch(itr->action)
		{
			case Action::Push :

				mStack.push_back(createState(itr->state));
				break;

			case Action::Pop :

				mStack.pop_back();
				break;

			case Action::Clear : 

				mStack.clear();
				break;

		}

	}

	mPendingList.clear();
}
