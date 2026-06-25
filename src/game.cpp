#include "../include/game.hpp"
#include "../include/game_packet.hpp"
#include "../include/state_manager.hpp"

void Game::launch() {
	StateManager::getInstance()->setState(StateManager::GameState::Menu);
	while (StateManager::getInstance()->getState() != StateManager::GameState::Close) {
		float time = m_game_clock.restart().asSeconds();
		if (time > 0.1f) time = 0.1f;
		processEvents();
		update(time);
		render();
	}
}

void Game::processEvents() {
	while (auto event = m_window.pollEvent()) {
		if (event->is<sf::Event::Closed>())
			StateManager::getInstance()->setState(StateManager::GameState::Close);
		if (auto keyboard_event = event->getIf<sf::Event::KeyPressed>()) {
			if (StateManager::getInstance()->getState() == StateManager::GameState::Menu) {
				menu.processEvents();
				if (keyboard_event->scancode == sf::Keyboard::Scancode::Enter) {
					m_is_host = (menu.getMenuOption() == 1) ? false : true;
					m_is_playing = true;
					m_socket.setBlocking(true);
					m_host_score = 0;
					m_client_score = 0;
					StateManager::getInstance()->setState(StateManager::GameState::Connecting);
					if (m_is_host) {
						m_status_text->setString("Waiting for the client...");
						m_status_text->setOrigin(m_status_text->getLocalBounds().size / 2.f);
						m_status_text->setPosition({ WINDOW_SIZE.x / 2.f, 25 });
						player1.setPosition({ PADDLE_SIZE.x, WINDOW_SIZE.y / 2.f });
						player2.setPosition({ WINDOW_SIZE.x - PADDLE_SIZE.x, WINDOW_SIZE.y / 2.f });
						player1.setColor(sf::Color::Red);
						player2.setColor(sf::Color::Blue);
						m_network_thread = std::thread(&Game::serverListen, this);
					} else {
						m_status_text->setString("Waiting for the server...");
						m_status_text->setOrigin(m_status_text->getLocalBounds().size / 2.f);
						m_status_text->setPosition({ WINDOW_SIZE.x / 2.f, 25 });
						player1.setPosition({ WINDOW_SIZE.x - PADDLE_SIZE.x, WINDOW_SIZE.y / 2.f });
						player2.setPosition({ PADDLE_SIZE.x, WINDOW_SIZE.y / 2.f });
						player1.setColor(sf::Color::Blue);
						player2.setColor(sf::Color::Red);
						m_network_thread = std::thread(&Game::clientListen, this);
					}
				}
			}
			if (keyboard_event->scancode == sf::Keyboard::Scancode::Escape) {
				StateManager::getInstance()->setState(StateManager::GameState::Disconnect);
			}
		}
	}
}

void Game::render() {
	m_window.clear();
	switch (StateManager::getInstance()->getState()) {
		case StateManager::GameState::Menu: 
			m_window.draw(menu); 
			break;
		case StateManager::GameState::Connecting: 
			m_window.draw(*m_status_text);
			break;
		case StateManager::GameState::Playing:
			m_window.draw(player1);
			m_window.draw(player2);
			m_window.draw(ball);
			m_window.draw(*m_score_text);
			break;
	}
	m_window.display();
}

void Game::update(float dt) {
	switch (StateManager::getInstance()->getState()) {
		case StateManager::GameState::Pause: break;
		case StateManager::GameState::Disconnect: 
			m_is_playing = false;
			if (m_network_thread.joinable())
				m_network_thread.join();
			m_socket.disconnect();
			StateManager::getInstance()->setState(StateManager::GameState::Menu);
			break;
		case StateManager::GameState::Close: 
			m_is_playing = false;
			if(m_network_thread.joinable())
				m_network_thread.join();
			m_window.close(); 
			break;
		case StateManager::GameState::Playing: 
			if (m_is_host) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) player1.moveUp(dt);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) player1.moveDown(dt);
				if (ball.getPosition().x <= BALL_SIZE) {
					++m_client_score;
					m_score_text->setString(std::to_string(m_host_score) + " : " + std::to_string(m_client_score));
					ball.reset();
				}
				if (ball.getPosition().x >= WINDOW_SIZE.x - BALL_SIZE) {
					++m_host_score;
					m_score_text->setString(std::to_string(m_host_score) + " : " + std::to_string(m_client_score));
					ball.reset();
				}
				sendDataToClient();
			} else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) player2.moveUp(dt);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) player2.moveDown(dt);
				if (ball.getPosition().x <= BALL_SIZE) ball.reset();
				if (ball.getPosition().x >= WINDOW_SIZE.x - BALL_SIZE) ball.reset();
				sendDataToServer();
			}
			if (m_incrase_clock.restart().asMilliseconds() >= 17) 
				ball.increaseSpeed();
			ball.update(player1, player2, dt);
			break;
	}
}

void Game::serverListen() {
	sf::TcpListener listener;
	if (listener.listen(m_port, m_address) != sf::Socket::Status::Done) {
		StateManager::getInstance()->setState(StateManager::GameState::Disconnect);
		return;
	}
	if (listener.accept(m_socket) != sf::Socket::Status::Done) {
		StateManager::getInstance()->setState(StateManager::GameState::Disconnect);
		return;
	}
	StateManager::getInstance()->setState(StateManager::GameState::Playing);
	m_socket.setBlocking(false);
	while (m_is_playing) {
		sf::Packet packet;
		while (m_socket.receive(packet) == sf::Socket::Status::Done) {
			std::lock_guard<std::mutex> guard(m_mutex);
			GamePacket data;
			packet >> data;
			if (player2.getPosition().y != data.paddle_position_y)
				player2.setPosition({ player2.getPosition().x, data.paddle_position_y });
		}
	}
	listener.close();
}

void Game::clientListen() {
	if(m_socket.connect(m_address, m_port) != sf::Socket::Status::Done) {
		StateManager::getInstance()->setState(StateManager::GameState::Disconnect);
		return;
	}
	StateManager::getInstance()->setState(StateManager::GameState::Playing);
	m_socket.setBlocking(false);
	while (m_is_playing) {
		sf::Packet packet;
		while (m_socket.receive(packet) == sf::Socket::Status::Done) {
			std::lock_guard<std::mutex> guard(m_mutex);
			GamePacket data;
			packet >> data;
			ball.setPosition(data.ball_position);
			if(player1.getPosition().y != data.paddle_position_y)
				player1.setPosition({ player1.getPosition().x, data.paddle_position_y });
			if (m_host_score != data.host_score) {
				m_host_score = data.host_score;
				m_score_text->setString(std::to_string(m_host_score) + " : " + std::to_string(m_client_score));

			}
			if (m_client_score != data.client_score) {
				m_client_score = data.client_score;
				m_score_text->setString(std::to_string(m_host_score) + " : " + std::to_string(m_client_score));

			}
		}
	}
	m_socket.disconnect();
}

void Game::sendDataToServer() {
	sf::Packet packet;
	packet << GamePacket(player2.getPosition().y);
	if (m_socket.send(packet) == sf::Socket::Status::Disconnected)
		StateManager::getInstance()->setState(StateManager::GameState::Disconnect);
	packet.clear();
}

void Game::sendDataToClient() {
	sf::Packet packet;
	packet << GamePacket(player1.getPosition().y, ball.getPosition(), m_host_score, m_client_score);
	if (m_socket.send(packet) == sf::Socket::Status::Disconnected)
		StateManager::getInstance()->setState(StateManager::GameState::Disconnect);
	packet.clear();
}