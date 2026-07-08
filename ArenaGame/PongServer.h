#ifndef PONGSERVER_H
#define PONGSERVER_H

#include <boost/asio.hpp>
#include <memory>
#include "ServerSession.h"
#include "GameConfig.h"
#include "GameState.h"
#include "Simulation.h"
#include "Types.h"

class PongServer
{
public:
	PongServer(::boost::asio::io_context& io, uint16_t port);

private:

	void accept();
	void startTick();
	void tickOnce();

	net::StateMessage makeStateMessage() const;

private:
	::boost::asio::io_context& m_io;
	::boost::asio::ip::tcp::acceptor m_acceptor;

	std::shared_ptr<ServerSession> m_p1{};
	std::shared_ptr<ServerSession> m_p2{};

	GameConfig m_config{};
	GameState m_state{};

	::boost::asio::steady_timer m_timer;

	const std::chrono::milliseconds m_tickMs{ 16 };
};

#endif // PONGSERVER_H

