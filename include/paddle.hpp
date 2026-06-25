#ifndef PADDLE_HPP
#define PADDLE_HPP

#include <SFML/Graphics.hpp>
#include "../include/constants.hpp"

class Paddle: public sf::Drawable {
private:
	sf::RectangleShape m_rect;

public:
	Paddle() {
		m_rect.setSize(PADDLE_SIZE);
		m_rect.setOrigin(m_rect.getSize() / 2.f);
	}
	void moveUp(float speed);
	void moveDown(float speed);
	void setPosition(sf::Vector2f position);
	void setColor(sf::Color color);
	sf::Vector2f getPosition();
	sf::Vector2f getSize();
	sf::RectangleShape getRect();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif // !PADDLE_HPP