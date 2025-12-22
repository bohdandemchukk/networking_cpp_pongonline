#include "Ball.h"



Ball::Ball(float radius, sf::Vector2f position)
{
	m_shape.setRadius(radius);
	m_shape.setPosition(position);
	
}

sf::Vector2f Ball::getDirection() {
	return m_ballDirection;
}

void Ball::setDirection(sf::Vector2f dir) {
	m_ballDirection = dir;
}


void Ball::clamp(float windowHeight) {

	auto pos { m_shape.getPosition() };
	float diameter{ m_shape.getRadius() * 2 };

	if (pos.y < 0.f) {
		pos.y = 0.f;
		bounceY();
	}

	if (pos.y > windowHeight - diameter) {
		pos.y = windowHeight - diameter;
		bounceY();
	}

	m_shape.setPosition(pos);
}

void Ball::draw(sf::RenderWindow& window) const {
	window.draw(m_shape);
}

sf::FloatRect Ball::getBounds() const {
	return m_shape.getGlobalBounds();
}

float Ball::getCenter() const {
	float position {m_shape.getPosition().y + m_shape.getRadius()};
	return position;
}

void Ball::bounceX() {
	m_ballDirection.x *= -1;
}

void Ball::bounceY() {
	m_ballDirection.y *= -1;
}

void Ball::update(float dt) {
	m_shape.move({ m_ballDirection.x * m_speed * dt, m_ballDirection.y * m_speed * dt });
}