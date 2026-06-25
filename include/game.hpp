#ifndef GAME_HPP
#define GAME_HPP

#include <thread>
#include <atomic>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "constants.hpp"
#include "paddle.hpp"
#include "ball.hpp"
#include "menu.hpp"

class Game {
private:
	sf::RenderWindow m_window;
	Paddle player1, player2;
	Ball ball;
	Menu menu;
	sf::Clock m_game_clock;
	sf::Clock m_incrase_clock;

	std::thread m_network_thread;
	std::mutex m_mutex;

	sf::TcpSocket m_socket;
	unsigned short m_port;
	sf::IpAddress m_address;
	bool m_is_host;
	int m_host_score;
	int m_client_score;
	std::atomic<bool> m_is_playing;

	sf::Font m_font;
	sf::Text* m_score_text;
	sf::Text* m_status_text;

public:
	Game()
		: m_is_host(false), m_address(sf::IpAddress::LocalHost), m_is_playing(true),
		m_port(8000), m_host_score(0), m_client_score(0) {
		m_window = sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Pong", sf::Style::Titlebar | sf::Style::Close);
		m_font.openFromFile(DEFAULT_FONT);
		m_score_text = new sf::Text(m_font, std::to_string(m_host_score) + " : " + std::to_string(m_client_score), 40);
		m_score_text->setFillColor(sf::Color::White);
		m_score_text->setOrigin(m_score_text->getLocalBounds().size / 2.f);
		m_score_text->setPosition({ WINDOW_SIZE.x / 2.f, 25 });

		m_status_text = new sf::Text(m_font, "", 40);
		m_status_text->setFillColor(sf::Color::White);
		m_window.setFramerateLimit(60);
	}
	void launch();

private:
	void processEvents();
	void render();
	void update(float dt);

	void serverListen();
	void clientListen();
	void sendDataToServer();
	void sendDataToClient();
};
#endif // !GAME_HPP