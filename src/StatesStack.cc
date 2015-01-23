#include "StatesStack.h"

StatesStack::StatesStack(State::Context context)
: mStack()
, mPendingList()
, mContext(context)
, mFactories() {
}

void StatesStack::update(sf::Time dt) {
	// Iterate from top to bottom, stop as soon as update() returns false
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StatesStack::draw() {
	// Draw all active states from bottom to top
    for(State::Ptr& State : mStack) {
        State->draw();
    }
}

void StatesStack::handleEvent(const sf::Event& event) {
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StatesStack::pushState(States::ID IDState) {
    mPendingList.push_back(PendingChange(Push, IDState));
}

void StatesStack::popState() {
	mPendingList.push_back(PendingChange(Pop));
}

void StatesStack::clearStates() {
	mPendingList.push_back(PendingChange(Clear));
}

bool StatesStack::isEmpty() const {
	return mStack.empty();
}

State::Ptr StatesStack::createState(States::ID IDState) {
    auto found = mFactories.find(IDState);
	assert(found != mFactories.end());

    return found->second();
}

void StatesStack::applyPendingChanges() {
    for (PendingChange change : mPendingList) {
        switch (change.mAction) {
            case Push:
                mStack.push_back(createState(change.mStateID));
                break;

            case Pop:
                mStack.pop_back();
                break;

            case Clear:
                mStack.clear();
                break;
        }
    }

	mPendingList.clear();
}

StatesStack::PendingChange::PendingChange(Action action, States::ID IDState)
: mAction(action)
, mStateID(IDState) {
}

void StatesStack::setContext(State::Context c) {
    mContext = c;
}
