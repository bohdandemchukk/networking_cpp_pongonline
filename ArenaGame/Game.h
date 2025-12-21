#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"


class Game
{
public:
	Game();

	void processEvents();
	void render();
	void update(float deltaTime);
	void run();
	
	sf::Vector2f calculateCollision(const Paddle& paddle, const Ball& ball);


private:

	sf::RenderWindow window;
	sf::Clock clock;

	Paddle leftPlayer;
	Paddle rightPlayer;
	Ball ball;

	bool ballLaunched{ false };


};

#endif // GAME_H

