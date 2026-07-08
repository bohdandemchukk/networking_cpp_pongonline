#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <boost/asio.hpp>
#include <optional>
#include <atomic>
#include "NetProtocol.h"

class NetClient
{
public:
	explicit NetClient(::boost::asio::io_context& io);

	void connect(const std::string& host, uint16_t port);

	uint8_t getPlayerId() const { return m_playerId; };

	void sendInput(float direction, bool servePressed);

	bool getHasState() const { return m_hasState.load(); };

	net::StateMessage getLastState() const { return m_lastState; }


private:
	void readHeader();
	void readPayload();
	void onMessage();

	template<class T>
	void writeMessage(const T& message);

private:
	::boost::asio::io_context& m_io;
	::boost::asio::ip::tcp::socket m_socket;

	net::MessageHeader m_inHeader{};
	std::vector<uint8_t> m_inPayload{};
	
	uint8_t m_playerId{};
	uint32_t m_sequence{};

	std::atomic<bool> m_hasState{};
	net::StateMessage m_lastState{};
};


#endif // NETCLIENT_H

