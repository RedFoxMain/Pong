#include "../include/menu.hpp"

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*m_connect_text, states);
	target.draw(*m_create_text, states);
}

void Menu::processEvents() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
		if (++m_option > 2) m_option = 1;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
		if (--m_option < 1) m_option = 2;

	if (m_option == 1) {
		m_connect_text->setFillColor(sf::Color::Green);
		m_create_text->setFillColor(sf::Color::White);
	} else {
		m_create_text->setFillColor(sf::Color::Green);
		m_connect_text->setFillColor(sf::Color::White);
	}
}

int Menu::getMenuOption() { return m_option; }