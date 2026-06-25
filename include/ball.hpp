#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/Graphics.hpp>
#include "paddle.hpp"
#include "constants.hpp"

class Ball: public sf::Drawable {
private:
	sf::CircleShape m_circle;
	sf::Vector2u m_window_size;
	sf::Vector2f m_dirs[4] = { 
		{ -1, 1 }, { 1, -1 }, 
		{ 1, 1 }, { -1, -1 }
	};
	sf::Vector2f m_dir;
	float m_speed;

public:
	Ball() {
		m_circle.setRadius(BALL_SIZE);
		m_circle.setOrigin({ m_circle.getRadius(), m_circle.getRadius() });
		reset();
	}
	void update(Paddle& player1, Paddle& player2, float delta_time);
	void setPosition(sf::Vector2f position);
	void reset();
	void increaseSpeed();
	sf::Vector2f getPosition();
	sf::CircleShape getCircle();
	float getRadius();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif // !BALL_HPP