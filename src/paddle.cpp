#include "../include/paddle.hpp"

void Paddle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_rect, states);
}
void Paddle::moveUp(float delta_time) { 
	if (m_rect.getPosition().y < PADDLE_SIZE.y / 2.f) return;
	m_rect.move({ 0, -PADDLE_SPEED * delta_time }); 
}
void Paddle::moveDown(float delta_time) { 
	if (m_rect.getPosition().y > WINDOW_SIZE.y - PADDLE_SIZE.y / 2.f) return;
	m_rect.move({ 0, PADDLE_SPEED * delta_time });
}
void Paddle::setPosition(sf::Vector2f position) { m_rect.setPosition(position); }
void Paddle::setColor(sf::Color color) { m_rect.setFillColor(color); }
sf::Vector2f Paddle::getPosition() { return m_rect.getPosition(); }
sf::Vector2f Paddle::getSize() { return m_rect.getSize(); }
sf::RectangleShape Paddle::getRect() { return m_rect; }