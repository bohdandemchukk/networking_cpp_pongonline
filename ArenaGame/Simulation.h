#ifndef SIMULATION_H
#define SIMULATION_H

#include "Types.h"
#include "GameState.h"
#include "GameConfig.h"

namespace Simulation {
	void resetMatch(const GameConfig& config, GameState& state);

	void step(const GameConfig& config, GameState& state, const Input& input, float dt);
}

#endif // SIMULATION_H

