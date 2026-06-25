#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "font_config.h"

class Menu: public sf::Drawable {
private:
	sf::Font* m_font;
	sf::Text* m_connect_text;
	sf::Text* m_create_text;
	int m_option;

public:
	Menu(): m_option(1) {
		m_font = new sf::Font(FONT_PATH);

		m_connect_text = new sf::Text(*m_font, "Connect", 28);
		m_connect_text->setOrigin(m_connect_text->getLocalBounds().size / 2.f);
		m_connect_text->setPosition(sf::Vector2f(WINDOW_SIZE.x / 2.f, (WINDOW_SIZE.y / 2.f) - 25));
		m_connect_text->setFillColor(sf::Color::Green);

		m_create_text = new sf::Text(*m_font, "Create", 28);
		m_create_text->setOrigin(m_create_text->getLocalBounds().size / 2.f);
		m_create_text->setPosition(sf::Vector2f(WINDOW_SIZE.x / 2.f, (WINDOW_SIZE.y / 2.f) + 25));
		m_create_text->setFillColor(sf::Color::White);
	}

	~Menu() {
		delete m_connect_text;
		delete m_create_text;
	}

	void processEvents();
	int getMenuOption();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif // !MENU_HPP