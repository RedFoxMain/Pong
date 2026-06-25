#include "../include/ball.hpp"
#include "../include/utils.hpp"
#include "../include/constants.hpp"

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_circle, states);
}

void Ball::update(Paddle& player1, Paddle& player2, float delta_time) {
	if ((m_circle.getPosition().y < m_circle.getRadius()) || (m_circle.getPosition().y > WINDOW_SIZE.y - m_circle.getRadius())) 
		m_dir.y *= -1;
	if (isCollided(player1, *this) || isCollided(player2, *this)) 
		m_dir.x *= -1;
	m_circle.move(m_dir * m_speed * delta_time);
}

void Ball::setPosition(sf::Vector2f position) { m_circle.setPosition(position); }
void Ball::reset() { 
	m_speed = BALL_SPEED;
	m_dir = m_dirs[rand() % 4];
	m_circle.setPosition(sf::Vector2f(WINDOW_SIZE) / 2.f); 
}
void Ball::increaseSpeed() { 
	m_speed  = std::clamp(m_speed + 0.5f, BALL_SPEED, BALL_SPEED * 2); 
}
sf::Vector2f Ball::getPosition() { return m_circle.getPosition(); }
sf::CircleShape Ball::getCircle() { return m_circle; }

float Ball::getRadius() { return m_circle.getRadius(); }