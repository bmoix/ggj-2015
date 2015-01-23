#ifndef STATES_STACK_H
#define STATES_STACK_H

#include "State.h"
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"
#include "Utils.h"

class StatesStack : private sf::NonCopyable {
	public:
        enum Action {
			Push,
			Pop,
			Clear,
		};

        typedef States::ID InfoState;

        explicit StatesStack(State::Context context);

        template <typename T>
        void registerState(States::ID IDState);

        bool isEmpty() const;

		void draw();
        void update(sf::Time dt);
        void handleEvent(const sf::Event& event);

        void popState();
        void clearStates();
        void pushState(States::ID IDState);

        void setContext(State::Context c);

	private:
        InfoState readNextState();

        State::Ptr createState(States::ID IDState);
        void applyPendingChanges();

        struct PendingChange {
            explicit PendingChange(Action action, States::ID stateID = States::None);

            Action mAction;
            States::ID mStateID;
		};

        std::vector<State::Ptr>	mStack;
        std::vector<PendingChange> mPendingList;

        State::Context mContext;
        std::map<States::ID, std::function<State::Ptr()>> mFactories;
};


template <typename T>
void StatesStack::registerState(States::ID IDState) {
    mFactories[IDState] = [this] () {
        return State::Ptr(new T(*this, mContext));
	};
}

#endif // STATES_STACK_H
