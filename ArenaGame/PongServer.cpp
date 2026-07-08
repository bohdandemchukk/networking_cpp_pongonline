#include "PongServer.h"
#include <iostream>

PongServer::PongServer(::boost::asio::io_context& io, uint16_t port)
	: m_io{ io }
	, m_acceptor{ io, ::boost::asio::ip::tcp::endpoint(::boost::asio::ip::tcp::v4(), port) }
	, m_timer {io}
{
	Simulation::resetMatch(m_config, m_state);
	accept();
	startTick();
}


void PongServer::accept() {
	m_acceptor.async_accept([this](boost::system::error_code errCode, ::boost::asio::ip::tcp::socket socket) {
		if (!errCode) {
			if (!m_p1 || !m_p1->isAlive()) {
				m_p1 = std::make_shared<ServerSession>(std::move(socket));
				m_p1->start(1);
				std::cout << "Player 1 connected\n";
			}
			else if (!m_p2 || !m_p2->isAlive()) {
				m_p2 = std::make_shared<ServerSession>(std::move(socket));
				m_p2->start(2);
				std::cout << "Player 2 connected\n";
			}
			else {
				socket.close();
				std::cout << "Rejected extra player. Server is full";
			}
		}
		accept();
	});
}

void PongServer::startTick() {
	m_timer.expires_after(m_tickMs);
	m_timer.async_wait([this](::boost::system::error_code errCode) {
		if (errCode) return;
		tickOnce();
		startTick();
	});
}

void PongServer::tickOnce() {
	Input input{};
	if (m_p1 && m_p1->isAlive()) {
		auto latestInput{ m_p1->getLatestInput() };
		input.leftDir = latestInput.direction;
		input.servePressed |= latestInput.serve;
	}

	if (m_p2 && m_p2->isAlive()) {
		auto latestInput{ m_p2->getLatestInput() };
		input.rightDir = latestInput.direction;
		input.servePressed |= latestInput.serve;
	}

	constexpr float dt{ 1.f / 60.f };
	Simulation::step(m_config, m_state, input, dt);

	net::StateMessage stateMessage{ makeStateMessage() };
	if (m_p1 && m_p1->isAlive()) m_p1->sendState(stateMessage);
	if (m_p2 && m_p2->isAlive()) m_p2->sendState(stateMessage);
}

net::StateMessage PongServer::makeStateMessage() const {
	net::StateMessage msg;

	msg.leftY = m_state.left.y;
	msg.rightY = m_state.right.y;

	msg.ballX = m_state.ball.pos.x;
	msg.ballY = m_state.ball.pos.y;

	msg.ballVX = m_state.ball.vel.x;
	msg.ballVY = m_state.ball.vel.y;

	msg.leftScore = m_state.score.left;
	msg.rightScore = m_state.score.right;

	msg.phase = static_cast<uint8_t>(m_state.phase);
	msg.countdownLeftSec = m_state.countdownLeftSec;
	msg.winner = m_state.winner;

	msg.tick = m_state.tick;

	return msg;
}