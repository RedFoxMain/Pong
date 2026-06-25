#include "../include/ui.hpp"

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (StateManager::getInstance()->getState() == GameState::MENU) {
		target.draw(*m_connect_to_room_text, states);
		target.draw(*m_create_room_text, states);
	}
	if (StateManager::getInstance()->getState() == GameState::WAITING)
		target.draw(*m_waiting_text, states);
}

void UI::processEvents() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
		if (++m_current_option > 2) m_current_option = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) 
		if (--m_current_option < 1) m_current_option = 2;
	if (m_current_option == 1) {
		m_connect_to_room_text->setFillColor(sf::Color::Green);
		m_create_room_text->setFillColor(sf::Color::White);
	} else {
		m_create_room_text->setFillColor(sf::Color::Green);
		m_connect_to_room_text->setFillColor(sf::Color::White);
	}
}

int UI::getSelectedOption() { return m_current_option; }