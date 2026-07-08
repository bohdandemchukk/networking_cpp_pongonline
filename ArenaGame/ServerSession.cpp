#include "ServerSession.h"

ServerSession::ServerSession(::boost::asio::ip::tcp::socket socket)
	: m_socket{ std::move(socket) } {}

void ServerSession::start(uint8_t playerId) {
	m_playerId = playerId;
	sendWelcome();
	readHeader();
}

uint8_t ServerSession::getPlayerId() const {
	return m_playerId;
}

PlayerLatestInput ServerSession::getLatestInput() const {
	return m_latestInput;
}

bool ServerSession::isAlive() const {
	return m_socket.is_open();
}

void ServerSession::sendState(const net::StateMessage& message) {
	enqueueWrite(message);
}

void ServerSession::sendWelcome() {
	net::WelcomeMessage welcomeMessage{};
	welcomeMessage.playerId = m_playerId;
	enqueueWrite(welcomeMessage);
}

template<class T>
void ServerSession::enqueueWrite(const T& message) {
	auto self{ shared_from_this() };
	const uint8_t* data { reinterpret_cast<const uint8_t*>(&message) };
	std::vector<uint8_t> buf(data, data + sizeof(T));
	::boost::asio::post(m_socket.get_executor(), [this, self, buf = std::move(buf)]() mutable {
		bool writing { !m_outbox.empty() };
		m_outbox.push_back(std::move(buf));
		if (!writing) doWrite();
	});
}

void ServerSession::doWrite() {
	auto self{ shared_from_this() };
	::boost::asio::async_write(
		m_socket, 
		::boost::asio::buffer(m_outbox.front()), 
		[this, self](::boost::system::error_code errCode, std::size_t) {
			if (errCode) { m_socket.close(); return; };
			m_outbox.pop_front();
			if (!m_outbox.empty()) doWrite();
	}
);
}

void ServerSession::readHeader() {
	auto self{ shared_from_this() };
	::boost::asio::async_read(m_socket, ::boost::asio::buffer(&m_inHeader, sizeof(m_inHeader)),
		[this, self](boost::system::error_code errCode, std::size_t) {
			if (errCode) { m_socket.close(); return; };
			if (m_inHeader.size > 1024) { m_socket.close(); return; };
			m_inPayload.resize(m_inHeader.size);
			readPayload();
		}
	);
}

void ServerSession::readPayload() {
	auto self{ shared_from_this() };
	::boost::asio::async_read(m_socket, ::boost::asio::buffer(m_inPayload.data(), m_inPayload.size()),
		[this, self](boost::system::error_code errCode, std::size_t) {
			if (errCode) { m_socket.close(); return; };
			onMessage();
			readHeader();
		}
	);
}

void ServerSession::onMessage() {
	if (m_inHeader.type == static_cast<uint8_t>(net::MessageType::Input)) {
		if (m_inPayload.size() == sizeof(net::InputMessage) - sizeof(net::MessageHeader)) {
			net::InputMessage message{};

			std::memcpy(reinterpret_cast<uint8_t*>(&message) + sizeof(net::MessageHeader), m_inPayload.data(), m_inPayload.size());

			m_latestInput.sequence = message.sequence;
			m_latestInput.direction = message.direction;
			m_latestInput.serve = ( message.serve != 0 );
		}
	}
}