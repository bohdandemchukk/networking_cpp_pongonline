#include "Paddle.h"


Paddle::Paddle(sf::Vector2f size, sf::Vector2f position) {
	shape.setSize(size);
	shape.setPosition(position);
}


float Paddle::getSizeY() const {
	return static_cast<float>(shape.getSize().y);
}

void Paddle::setDirection(float dir) {
	direction = dir;
}

void Paddle::update(float dt) {
	shape.move({ static_cast<float>(shape.getPosition().x), direction * speed * dt} );
}

void Paddle::clamp(float windowHeight) {
	if (shape.getPosition().y < 0.f) {
		shape.setPosition({ static_cast<float>(shape.getPosition().x), 0.f });
	}

	if (shape.getPosition().y > windowHeight - getSizeY()) {
		shape.setPosition({ static_cast<float>(shape.getPosition().x), windowHeight - getSizeY() });
	}
}

void Paddle::draw(sf::RenderWindow& window) const {
	window.draw(shape);
}

sf::FloatRect Paddle::getBounds() const {
	return shape.getGlobalBounds();
}