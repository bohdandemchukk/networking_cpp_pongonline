#ifndef SFMLRENDERER_H
#define SFMLRENDERER_H

#include "GameConfig.h"
#include "GameState.h"
#include <SFML/Graphics.hpp>

class SfmlRenderer
{
public:
	SfmlRenderer(const GameConfig& config);

	void draw(sf::RenderWindow& window, const GameState& state);

private:
	GameConfig m_config;

	sf::RectangleShape m_left;
	sf::RectangleShape m_right;
	sf::CircleShape m_ball;

	sf::Font m_font;
	sf::Text m_scoreText;
	sf::Text m_winText;
	sf::Text m_hintText;
};


#endif // SFMLRENDERER_H

