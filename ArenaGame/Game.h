#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"

enum class GameState {
	WaitingServe,
	Countdown,
	Playing,
	GameOver
};



class Game
{
public:
	Game();

	void serveBall();
	void handleInput();
	void findCollision();
	void updateEntities(float dt);
	void clampPlayers();
	void clampBall();
	void clampEntities();
	void processEvents();
	void render();
	void update(float dt);
	void run();

	void checkGoal();
	void resetBall();
	void checkWin();
	void resetMatch();
	void resetPaddles();
	void resetScore();
	void updateScoreText();
	void startCountdown();


	
	sf::Vector2f calculateCollision(const Paddle& paddle, const Ball& ball);


private:

	sf::RenderWindow window;
	float windowHeight{};
	sf::Clock clock;

	Paddle leftPlayer;
	Paddle rightPlayer;
	Ball ball;

	GameState m_state{};
	int leftScore{0};
	int rightScore{0};
	const int POINTS_TO_WIN{ 5 };
	int playerWon{};

	sf::Font m_font;

	sf::Text m_scoreText;
	sf::Text m_winText;
	sf::Text m_hintText;

	sf::Clock m_countdownClock;
	bool m_isServeCountdown{};

};

#endif // GAME_H

