#include "NetworkGame.h"


NetworkGame::NetworkGame()
	: m_window{ sf::VideoMode{ {800, 600} }, "Pong Multiplayer" }
	, m_renderer(m_config)
	, m_client(m_io)
{
	m_window.setFramerateLimit(240);

	m_client.connect("127.0.0.1", 7777);

	m_netThread = std::thread([this]() {
		m_io.run();
	});
}

void NetworkGame::processEvents() {
	while (const auto event{ m_window.pollEvent() }) {
		if (event->is<sf::Event::Closed>()) {
			m_window.close();
		}
	}
}

void NetworkGame::update(float dt) {
	uint8_t playerId = m_client.getPlayerId();
	if (playerId == 0) return;

	Input input{ m_input.sample(playerId) };

	float direction{ playerId == 1 ? input.leftDir : input.rightDir };

	m_client.sendInput(direction, input.servePressed);

	if (m_client.getHasState()) {
		auto state{ m_client.getLastState() };

		m_renderState.left.y = state.leftY;
		m_renderState.right.y = state.rightY;

		m_renderState.ball.pos.x = state.ballX;
		m_renderState.ball.pos.y = state.ballY;

		m_renderState.ball.vel.x = state.ballVX;
		m_renderState.ball.vel.y = state.ballVY;

		m_renderState.score.left = state.leftScore;
		m_renderState.score.right = state.rightScore;

		m_renderState.phase = static_cast<Phase>(state.phase);
		m_renderState.countdownLeftSec = state.countdownLeftSec;
		m_renderState.winner = state.winner;
		m_renderState.tick = state.tick;
	}
}

void NetworkGame::render() {
	m_window.clear(sf::Color::Black);
	m_renderer.draw(m_window, m_renderState);
	m_window.display();
}

void NetworkGame::run() {
	while (m_window.isOpen()) {
		processEvents();
		float dt{ m_clock.restart().asSeconds() };
		update(dt);
		render();
	}
}