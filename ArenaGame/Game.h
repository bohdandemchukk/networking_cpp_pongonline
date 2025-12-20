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



private:

	sf::RenderWindow window;
	sf::Clock clock;

	Paddle leftPlayer;
	Paddle rightPlayer;
	Ball ball;

};

#endif // GAME_H

