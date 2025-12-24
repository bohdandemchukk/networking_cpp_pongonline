#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Types.h"
#include "GameConfig.h"

struct GameState {
	Paddle left{};
	Paddle right{};
	Ball ball{};
	Score score{};
	Phase phase{ Phase::WaitingServe };

	float countdownLeftSec{ 0.f };
	int winner{ 0 }; // 1 - left, 2 - right

	uint32_t tick{};
};


#endif GAMESTATE_H