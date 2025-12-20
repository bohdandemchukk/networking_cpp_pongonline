#include "Game.h"


Game::Game()
	: window(sf::VideoMode({ 800, 600 }), "Pong"),
	leftPlayer({ 10.f, 100.f }, { 50.f, 250.f }), 
	rightPlayer({ 10.f, 100.f }, { 740.f, 250.f }),
	ball(10.f, { 400.f, 300.f } )
{
	window.setFramerateLimit(60);
}

void Game::update(float deltaTime) {

	float leftDir{ 0.f };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) leftDir -= 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) leftDir += 1.f;
	rightPlayer.setDirection(leftDir);

	float rightDir{ 0.f };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) rightDir -= 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) rightDir += 1.f;
	rightPlayer.setDirection(rightDir);

	float windowHeight{ static_cast<float>(window.getSize().y) };
	leftPlayer.clamp(windowHeight);
	rightPlayer.clamp(windowHeight);
}

void Game::run() {
	while (window.isOpen()) {
		processEvents();
		float dt{ clock.restart().asSeconds() };
		update(dt);
		render();
	}
}



void Game::processEvents() {
	while (const auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
	}
}

void Game::render() {
	window.clear(sf::Color::Black);
	leftPlayer.draw(window);
	ball.draw(window);
	rightPlayer.draw(window);
	window.display();
}