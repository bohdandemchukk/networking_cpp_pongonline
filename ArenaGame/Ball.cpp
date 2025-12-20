#include "Ball.h"


Ball::Ball(float radius, sf::Vector2f position) {
	shape.setRadius(radius);
	shape.setPosition(position);
}


void Ball::draw(sf::RenderWindow& window) const {
	window.draw(shape);
}