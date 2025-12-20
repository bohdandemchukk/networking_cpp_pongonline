#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball
{

public:
	Ball(float radius, sf::Vector2f position);

	void draw(sf::RenderWindow& window) const;


	void setDirection(float dir);
	void update(float dt);

	sf::FloatRect getBounds();

private:
	sf::CircleShape shape;
	
	float direction{ 0.f };
	float speed{ 400.f };
};


#endif // BALL_H
