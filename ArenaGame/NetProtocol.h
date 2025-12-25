#ifndef NETPROTOCOL_H
#define NETPROTOCOL_H

#include <cstdint>

namespace net {
	enum class MessageType: uint8_t {
		Welcome = 1,
		Input = 2,
		State = 3
	};

#pragma pack(push, 1)

	struct MessageHeader {
		uint8_t type;
		uint16_t size;
	};

	struct WelcomeMessage {
		MessageHeader header{ static_cast<uint8_t>(MessageType::Welcome), static_cast<uint16_t>(sizeof(WelcomeMessage)) - static_cast<uint16_t>(sizeof(MessageHeader)) };
		uint8_t playerId;
	};

	struct InputMessage {
		MessageHeader header{ static_cast<uint8_t>(MessageType::Input), static_cast<uint16_t>(sizeof(InputMessage)) - static_cast<uint16_t>(sizeof(MessageHeader)) };
		uint8_t playerId;
		uint32_t sequence;
		float direction; // fix later
		uint8_t serve;

	};

	struct StateMessage {
		MessageHeader header{ static_cast<uint8_t>(MessageType::State), static_cast<uint16_t>(sizeof(StateMessage)) - static_cast<uint16_t>(sizeof(MessageHeader)) };
		
		float leftY;
		float rightY;

		float ballX;
		float ballY;
		float ballVX;
		float ballVY;

		int32_t leftScore;
		int32_t rightScore;

		uint8_t phase;
		float countdownLeftSec;
		int32_t winner;

		uint32_t tick;
	};

#pragma pack(pop)

	static_assert(sizeof(MessageHeader) == 3);
}


#endif // NETPROTOCOL_H