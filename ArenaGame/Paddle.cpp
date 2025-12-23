#include "Paddle.h"
#include <algorithm>


Paddle::Paddle(sf::Vector2f size, sf::Vector2f position) {
	m_shape.setSize(size);
	m_shape.setPosition(position);
}


float Paddle::getCenter() const {
	float position { m_shape.getPosition().y + m_shape.getSize().y / 2 };
	return position;
}


float Paddle::getDirection() {
	return m_direction;
}

float Paddle::getSizeY() const {
	return m_shape.getSize().y;
}

void Paddle::setDirection(float dir) {
	m_direction = dir;
}

void Paddle::update(float dt) {
	m_shape.move({ 0.f, m_direction * m_speed * dt} );
}

void Paddle::clamp(float windowHeight) {

	auto pos{ m_shape.getPosition() };
	float paddleLength{ m_shape.getSize().y };
	
	pos.y = std::clamp(pos.y, 0.f, windowHeight - paddleLength);

	m_shape.setPosition(pos);
}

void Paddle::draw(sf::RenderWindow& window) const {
	window.draw(m_shape);
}

sf::FloatRect Paddle::getBounds() const {
	return m_shape.getGlobalBounds();
}

void Paddle::setPosition(sf::Vector2f pos) {
	m_shape.setPosition(pos);
}