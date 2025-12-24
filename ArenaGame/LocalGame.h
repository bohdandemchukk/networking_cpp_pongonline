#ifndef LOCALGAME_H
#define LOCALGAME_H

#include "GameState.h"
#include "GameConfig.h"
#include "SfmlRenderer.h"
#include "SfmlInput.h"
#include "Types.h"
#include <SFML/Graphics.hpp>

class LocalGame
{
public:
	LocalGame();
	void processEvents();
	void render();
	void update(float dt);
	void run();



private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;

	GameConfig m_config;
	GameState m_state;

	SfmlInput m_input;
	SfmlRenderer m_renderer;

	float m_accumulator{ 0.f };
};

#endif // LOCALGAME_H

