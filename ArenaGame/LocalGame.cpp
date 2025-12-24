#include "LocalGame.h"
#include "Simulation.h"
#include <cmath>
#include <iostream>
#include <algorithm>


LocalGame::LocalGame()
	: m_window{ sf::VideoMode{ { 800, 600 } }, "Pong" },
	m_renderer(m_config)
{
	m_window.setFramerateLimit(120);
	Simulation::resetMatch(m_config, m_state);
}

void LocalGame::processEvents() {
	while (const auto event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_window.close();
	}
}

void LocalGame::update(float dt) {
	Input input{};

	Input left{ m_input.sample(1) };
	input.leftDir = left.leftDir;
	input.servePressed |= left.servePressed;

	Input right{ m_input.sample(2) };
	input.rightDir = right.rightDir;
	input.servePressed |= right.servePressed;

	Simulation::step(m_config, m_state, input, dt);
}

void LocalGame::render() {
	m_window.clear(sf::Color::Black);

	m_renderer.draw(m_window, m_state);
	
	m_window.display();
}

void LocalGame::run() {
	while (m_window.isOpen()) {
		processEvents();
		float dt{ m_clock.restart().asSeconds() };
		update(dt);
		render();
	}
}