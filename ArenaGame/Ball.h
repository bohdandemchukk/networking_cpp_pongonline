#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball
{
public:

	Ball(float radius, sf::Vector2f position);

	void draw(sf::RenderWindow& window) const;

	sf::Vector2f getDirection();

	void setDirection(sf::Vector2f dir);
	void update(float dt);

	void bounceX();
	void bounceY();

	sf::FloatRect getBounds() const;
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition() const;
	float getRadius() const;

	void clamp(float windowHeight);

	float getCenter() const;

private:
	sf::CircleShape m_shape;
	
	sf::Vector2f m_ballDirection{};

	float m_speed{ 400.f };
};


#endif // BALL_H
