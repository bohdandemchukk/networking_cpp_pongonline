#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle
{
public:
	Paddle(sf::Vector2f size, sf::Vector2f position);

	float getDirection();
	void setDirection(float dir);
	
	void update(float dt);

	void setPosition(sf::Vector2f pos);

	sf::FloatRect getBounds() const;

	float getCenter() const;
	
	float getSizeY() const;

	void clamp(float deltaTime);
	
	void draw(sf::RenderWindow& window) const;

private:
	float m_speed{ 300.f };
	sf::RectangleShape m_shape{};

	float m_direction{ 0.f };
};


#endif // PADDLE_H

