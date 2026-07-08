#include "NetClient.h"
#include <iostream>

NetClient::NetClient(boost::asio::io_context& io)
	: m_io{ io }, m_socket{ io } {}

void NetClient::connect(const std::string& host, uint16_t port) {

	::boost::asio::ip::tcp::resolver resolver{ m_io };
	auto endpoints{ resolver.resolve(host, std::to_string(port)) };
	::boost::asio::connect(m_socket, endpoints);
	readHeader();
}

void NetClient::readHeader() {
	::boost::asio::async_read(
		m_socket,
		::boost::asio::buffer(&m_inHeader, sizeof(m_inHeader)),
		[this](::boost::system::error_code errCode, std::size_t) {
			if (errCode) { m_socket.close(); return; };
			m_inPayload.resize(m_inHeader.size);
			readPayload();
		});
}

void NetClient::readPayload() {
	::boost::asio::async_read(
		m_socket,
		::boost::asio::buffer(m_inPayload.data(), m_inPayload.size()),
		[this](::boost::system::error_code errCode, std::size_t) {
			if (errCode) { m_socket.close(); return; };
			onMessage();
			readHeader();
		}
	);
}

void NetClient::onMessage() {
	if (m_inHeader.type == static_cast<uint8_t>(net::MessageType::Welcome)) {
		if (m_inPayload.size() == sizeof(net::WelcomeMessage) - sizeof(net::MessageHeader)) {
			net::WelcomeMessage welcomeMessage{};
			std::memcpy(reinterpret_cast<uint8_t*>(&welcomeMessage) + sizeof(net::MessageHeader), m_inPayload.data(), m_inPayload.size());
			m_playerId = welcomeMessage.playerId;
		}
	}
	else if (m_inHeader.type == static_cast<uint8_t>(net::MessageType::State)) {
		if (m_inPayload.size() == sizeof(net::StateMessage) - sizeof(net::MessageHeader)) {
			net::StateMessage stateMessage{};
			std::memcpy(reinterpret_cast<uint8_t*>(&stateMessage) + sizeof(net::MessageHeader), m_inPayload.data(), m_inPayload.size());
			m_lastState = stateMessage;
			m_hasState.store(true);
		}
	}
}

template <class T>
void NetClient::writeMessage(const T& message) {
	::boost::asio::async_write(
		m_socket,
		::boost::asio::buffer(&message, sizeof(T)),
		[this](::boost::system::error_code errCode, std::size_t) {
			if (errCode) { m_socket.close(); };
		});
}

void NetClient::sendInput(float direction, bool servePressed) {
	if (!m_socket.is_open() || m_playerId == 0) { return; };

	net::InputMessage inputMessage{};

	inputMessage.playerId = m_playerId;
	inputMessage.sequence = ++m_sequence;

	std::cout << "SEQ: " << m_sequence << "\n";


	inputMessage.direction = direction;
	inputMessage.serve = servePressed ? 1 : 0;

	writeMessage(inputMessage);
}