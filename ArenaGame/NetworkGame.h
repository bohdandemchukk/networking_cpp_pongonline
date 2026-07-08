#ifndef NETWORKGAME_H
#define NETWORKGAME_H
#include <SFML/Graphics.hpp>
#include "GameConfig.h"
#include "GameState.h"
#include "SfmlInput.h"
#include "SfmlRenderer.h"
#include <boost/asio.hpp>
#include "NetClient.h"


class NetworkGame
{
public:
	NetworkGame();
	void run();

private:
	void processEvents();
	void update(float dt);
	void render();

private:
	sf::RenderWindow m_window{};
	sf::Clock m_clock{};

	GameConfig m_config{};
	GameState m_renderState{};

	SfmlInput m_input{};
	SfmlRenderer m_renderer;

	::boost::asio::io_context m_io;
	NetClient m_client;

	std::thread m_netThread{};



};

#endif // NETWORKGAME_H