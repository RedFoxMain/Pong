#ifndef GAME_PACKET_HPP
#define GAME_PACKET_HPP

#include <SFML/Network.hpp>

struct GamePacket {
	float paddle_position_y;
	sf::Vector2f ball_position;
	int host_score, client_score;
	GamePacket() = default;
	GamePacket(float paddle_position_y)
		: paddle_position_y(paddle_position_y) {}
	GamePacket(float paddle_position_y, const sf::Vector2f& ball_position, int host_score, int client_score)
		: paddle_position_y(paddle_position_y), ball_position(ball_position), host_score(host_score), client_score(client_score) {}
};

sf::Packet& operator<<(sf::Packet& packet, const GamePacket& data) {
	return packet << data.paddle_position_y << data.ball_position.x << data.ball_position.y << data.host_score << data.client_score;
}

sf::Packet& operator>>(sf::Packet& packet, GamePacket& data) {
	return packet >> data.paddle_position_y >> data.ball_position.x >> data.ball_position.y >> data.host_score >> data.client_score;
}
#endif // !GAME_PACKET_HPP