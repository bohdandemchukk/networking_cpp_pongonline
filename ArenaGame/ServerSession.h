#ifndef SERVERSESSION_H
#define SERVERSESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <deque>
#include "NetProtocol.h"



struct PlayerLatestInput {
	uint32_t sequence{ 0 };
	float direction {0.f};
	bool serve{ false };
};

class ServerSession : public std::enable_shared_from_this<ServerSession> {
public:
	explicit ServerSession(::boost::asio::ip::tcp::socket socket);

	void start(uint8_t playerId);
	uint8_t getPlayerId() const;
	PlayerLatestInput getLatestInput() const;
	bool isAlive() const;
	void sendState(const net::StateMessage& message);

private:
	void sendWelcome();

	template<class T>
	void enqueueWrite(const T& message);
	void doWrite();
	void readHeader();
	void readPayload();
	void onMessage();

private:
	::boost::asio::ip::tcp::socket m_socket;
	uint8_t m_playerId{ 0 };

	PlayerLatestInput m_latestInput{};

	net::MessageHeader m_inHeader{};
	std::vector<uint8_t> m_inPayload{};

	std::deque<std::vector<uint8_t>> m_outbox{};

};
#endif // SERVERSESSION_H
