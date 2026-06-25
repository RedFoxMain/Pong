#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <memory>
#include <mutex>


class StateManager {
public:
	enum GameState {
		Menu, Playing, Pause, Close, Disconnect, Connecting
	};

private:
	static std::unique_ptr<StateManager> m_instance;
	static std::once_flag m_init_flag;
	GameState m_current_state;

public:
	StateManager() = default;
	StateManager(const StateManager&) = delete;
	StateManager& operator=(const StateManager&) = delete;

	static StateManager* getInstance();
	void setState(const GameState& state);
	GameState getState();
};
#endif // !STATE_MANAGER_HPP