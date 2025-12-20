#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball
{

public:
	Ball(float radius, sf::Vector2f position);

	void draw(sf::RenderWindow& window) const;

	sf::FloatRect getBounds() {
		return shape.getGlobalBounds();
	}

private:
	sf::CircleShape shape;
	float speed{ 400.f };
};


#endif // BALL_H
