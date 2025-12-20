#include "Ball.h"


Ball::Ball(float radius, sf::Vector2f position) {
	shape.setRadius(radius);
	shape.setPosition(position);
}


void Ball::draw(sf::RenderWindow& window) const {
	window.draw(shape);
}

sf::FloatRect Ball::getBounds() {
	return shape.getGlobalBounds();
}

void Ball::setDirection(float dir) {
	direction = dir;
}

void Ball::update(float dt) {
	shape.move({ 0.f, direction * speed * dt });
}