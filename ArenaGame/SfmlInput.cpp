#include "SfmlInput.h"


Input SfmlInput::sample(int clientId) {
	Input input{};

	if (clientId == 1) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) input.leftDir -= 1.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) input.leftDir += 1.f;
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) input.rightDir -= 1.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) input.rightDir += 1.f;
	}

	input.servePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	return input;
}