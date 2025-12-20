#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle
{
public:
	Paddle(sf::Vector2f size, sf::Vector2f position);

	void setDirection(float dir);
	
	void update(float dt);


	sf::FloatRect getBounds() const;
	
	float getSizeY() const;
	void clamp(float deltaTime);
	
	void draw(sf::RenderWindow& window) const;

private:
	float speed{ 300.f };
	sf::RectangleShape shape{};

	float direction{ 0.f };
};


#endif // PADDLE_H

