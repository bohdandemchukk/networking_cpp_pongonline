#include "Game.h"
#include "NormalizeUtil.h"
#include "DebugUtil.h"
#include <cmath>
#include <iostream>
#include <algorithm>


Game::Game()
	: window{ sf::VideoMode{ { 800, 600 } }, "Pong" },
	leftPlayer{ { 10.f, 100.f }, { 50.f, 250.f } }, 
	rightPlayer{ { 10.f, 100.f }, { 740.f, 250.f } },
	ball{ 10.f, { 400.f, 300.f } },
	m_state{ GameState::WaitingServe },
	m_font{ "assets/arial.ttf" },
	m_scoreText{ m_font, "", 32 },
	m_winText{ m_font, "", 64 },
	m_hintText{ m_font, "Press Space to Continue", 24 }
{
	window.setFramerateLimit(60);
	windowHeight = static_cast<float>(window.getSize().y);
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setPosition({ 375.f, 10.f });
	m_winText.setPosition({ 390.f, 60.f });
	m_winText.setFillColor(sf::Color::White);
	m_winText.setStyle(sf::Text::Bold);
	m_hintText.setFillColor(sf::Color::White);
	m_hintText.setPosition({ 270.f, 550.f });
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

void Game::checkGoal() {
	
	auto pos{ ball.getPosition() };

	if (pos.x < 0.f) {
		rightScore += 1;
		std::cout << "Right scored";
		resetBall();
		checkWin();
		updateScoreText();
		

	}

	if (pos.x > window.getSize().x) {
		leftScore += 1;
		std::cout << pos.x;
		std::cout << "Left scored";
		resetBall();
		checkWin();
		updateScoreText();
		
		
	}
}

void Game::resetBall() {
	ball.setPosition({ 400.f, 300.f });
	ball.setDirection({ 0.f, 0.f });
	m_state = GameState::WaitingServe;
}

void Game::resetPaddles() {
	leftPlayer.setPosition({ 50.f, 250.f });
	rightPlayer.setPosition({ 740.f, 250.f });
}

void Game::resetScore() {
	leftScore = 0;
	rightScore = 0;
}

void Game::resetMatch() {
	resetBall();
	resetPaddles();
	resetScore();
	updateScoreText();
}

void Game::updateScoreText() {
	m_scoreText.setString( std::to_string(leftScore) + " : " + std::to_string(rightScore) );
}

	

void Game::serveBall() {
	
	const float x = (std::rand() % 2 == 0) ? -1.f : 1.f;
	const float y = (std::rand() % 2 == 0) ? -0.5f : 0.5f;

	sf::Vector2f dir{ x, y };

	ball.setDirection(normalize(dir));

	m_state = GameState::Playing;
}

void Game::checkWin() {
	if (leftScore >= POINTS_TO_WIN) { 
		playerWon = 1;
		m_winText.setString("LEFT PLAYER WIN");
		m_state = GameState::GameOver;
	}
	if (rightScore >= POINTS_TO_WIN) {
		playerWon = 2;
		m_winText.setString("RIGHT PLAYER WIN");
		m_state = GameState::GameOver;
	}
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

void Game::startCountdown() {
	m_state = GameState::Countdown;
	m_countdownClock.restart();
}

void Game::update(float dt) {

	switch (m_state) {

	case GameState::WaitingServe:

		handleInput();
		updateEntities(dt);
		updateScoreText();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
			startCountdown();
		}
		break;

	case GameState::Countdown: {

		handleInput();
		updateEntities(dt);

		float timeElapsed{ m_countdownClock.getElapsedTime().asSeconds() };
		int secondsLeft{ 3 - static_cast<int>(timeElapsed) };
		m_winText.setString(std::to_string(secondsLeft));
		if (timeElapsed >= 3.f) {
			m_isServeCountdown = false;
			serveBall();
		}

		break;
	}
			

	case GameState::Playing:
		handleInput();
		updateEntities(dt);
		findCollision();
		clampEntities();
		checkGoal();
		break;

	case GameState::GameOver:

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
			resetMatch();
			m_state = GameState::WaitingServe;
		}
		break;
	}
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

	if (m_state == GameState::Countdown) {
		window.draw(m_winText);
	}
	window.draw(m_scoreText);
	leftPlayer.draw(window);
	ball.draw(window);
	rightPlayer.draw(window);
	if (m_state == GameState::GameOver) {
		window.draw(m_winText);
		window.draw(m_hintText);
	}
	window.display();
}