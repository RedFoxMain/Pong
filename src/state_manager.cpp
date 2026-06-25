#include "../include/state_manager.hpp"

std::unique_ptr<StateManager> StateManager::m_instance = nullptr;
std::once_flag StateManager::m_init_flag;

StateManager* StateManager::getInstance() {
	std::call_once(m_init_flag, []() {
		m_instance.reset(new StateManager());
	});
	return m_instance.get();
}

void StateManager::setState(const GameState& state) { m_current_state = state; }
StateManager::GameState StateManager::getState() { return m_current_state; }