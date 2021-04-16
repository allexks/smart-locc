#include "StateManager.hpp"

StateManager::StateManager()
    : state(INITIAL)
    , lastStateUpdateTime(0)
    , currentTime(0)
{}

void StateManager::setCurrentTime(DeviceTime time) {
    currentTime = time;

    // handle timeouts

    DeviceTime timeout = getTimeoutForCurrentState();
    if (timeout == -1) return;

    DeviceTime elapsed = currentTime - lastStateUpdateTime;
    if (elapsed >= timeout) {
        // state timed out
        setState(IDLE);
    }
}

DeviceState StateManager::getState() const {
    return state;
}

const char* StateManager::getStateMessage() const {
    // TODO: Finaliza texts
    switch (state)
    {
    case INITIAL:
        return "Booting up...";
    case IDLE:
        return "Idle";
    case AUTHORIZED:
        return "Welcome!";
    case AUTH_FAILURE:
        return "Could not authorize.";
    case WAITING_ADMIN_AUTH:
        return "Please use an admin key...";
    case WAITING_NEW_KEY_AUTH:
        return "Please use the new key...";
    case NEW_KEY_AUTH_SUCCESS:
        return "New key added! Welcome!";
    }
}

bool StateManager::isLocked() const {
    switch (state)
    {
    case AUTHORIZED:
    case NEW_KEY_AUTH_SUCCESS:
        return false;

    default:
        return true;
    }
}

void StateManager::setupDidFinish() {
    setState(IDLE);
}

void StateManager::setState(DeviceState newState) {
    state = newState;
    lastStateUpdateTime = currentTime;
}

DeviceTime StateManager::getTimeoutForCurrentState() {
    switch (state)
    {
    case INITIAL:
    case IDLE:
        return -1; // never

    default:
        return 5000; // five seconds default
    }
}