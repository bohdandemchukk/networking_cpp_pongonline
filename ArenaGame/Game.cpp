#include "Game.h"
#include "NormalizeUtil.h"
#include "DebugUtil.h"
#include <cmath>
#include <iostream>
#include <algorithm>


Game::Game()
	: window(sf::VideoMode({ 800, 600 }), "Pong"),
	leftPlayer({ 10.f, 100.f }, { 50.f, 250.f }), 
	rightPlayer({ 10.f, 100.f }, { 740.f, 250.f }),
	ball(10.f, { 400.f, 300.f })
{
	window.setFramerateLimit(60);
	windowHeight = static_cast<float>(window.getSize().y);
	std::srand(static_cast<unsigned>(std::time(nullptr)));	
}

sf::Vector2f Game::calculateCollision(const Paddle& paddle, const Ball& ball) {
	
	const float offset{ ball.getCenter() - paddle.getCenter() };
	const float maxOffset{ paddle.getSizeY() / 2 };
	const float normalizedOffset{ std::clamp(offset / maxOffset, -1.f, 1.f)};

	constexpr int MAX_ANGLE{ 60 };
	constexpr float PI{ 3.14f };
	constexpr float DEG_TO_RADIANS{ PI / 180 };

	const float angle{ normalizedOffset * static_cast<float>(MAX_ANGLE) };

	const float radians{ static_cast<float>(angle * DEG_TO_RADIANS) };

	float dirX{ std::cos(radians) };
	float dirY{ std::sin(radians) };

	// cos^2(x) + sin^2(x) = 1

	return { dirX, dirY };
}

void Game::findCollision() {
	if (leftPlayer.getBounds().findIntersection(ball.getBounds())) {
		sf::Vector2f newDirection{ calculateCollision(leftPlayer, ball) };
		ball.setDirection(newDirection);
	}

	if (rightPlayer.getBounds().findIntersection(ball.getBounds())) {
		sf::Vector2f newDirection{ calculateCollision(rightPlayer, ball) };
		newDirection.x *= -1;
		ball.setDirection(newDirection);
	}
}
	

void Game::startGame() {
	
		const float x = (std::rand() % 2 == 0) ? -1.f : 1.f;
		const float y = (std::rand() % 2 == 0) ? -0.5f : 0.5f;

		sf::Vector2f dir{ x, y };

		ball.setDirection(normalize(dir));

		ballLaunched = true;
}

void Game::handleInput() {
	float leftDir{ 0.f };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) leftDir -= 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) leftDir += 1.f;
	leftPlayer.setDirection(leftDir);

	float rightDir{ 0.f };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) rightDir -= 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) rightDir += 1.f;
	rightPlayer.setDirection(rightDir);
}

void Game::updateEntities(float dt) {
	leftPlayer.update(dt);
	rightPlayer.update(dt);
	ball.update(dt);
}

void Game::clampPlayers() {
	leftPlayer.clamp(windowHeight);
	rightPlayer.clamp(windowHeight);
	
}

void Game::clampBall() {
	ball.clamp(windowHeight);
}

void Game::clampEntities() {
	clampPlayers();
	clampBall();

}

void Game::update(float dt) {

	if (!ballLaunched && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		startGame();
	}
		handleInput();
		updateEntities(dt);
		findCollision();
		clampEntities();
}

void Game::processEvents() {
	while (const auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
	}
}

void Game::run() {
	

	while (window.isOpen()) {
		processEvents();
		float dt{ clock.restart().asSeconds() };
		update(dt);
		render();
	}
}

void Game::render() {
	window.clear(sf::Color::Black);
	leftPlayer.draw(window);
	ball.draw(window);
	rightPlayer.draw(window);
	window.display();
}