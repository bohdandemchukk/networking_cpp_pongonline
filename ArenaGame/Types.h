#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

struct Vec2 {
	float x{};
	float y{};
};

struct Paddle {
	float y{};
	float dir{};
};

struct Ball {
	Vec2 pos{};
	Vec2 vel{};
};

struct Score {
	int left{};
	int right{};
};

enum class Phase: uint8_t {
	WaitingServe = 0,
	Countdown = 1,
	Playing = 2,
	GameOver = 3
};

struct Input {
	float leftDir{ 0.f };
	float rightDir{ 0.f };
	bool servePressed{};
};

#endif // TYPES_H