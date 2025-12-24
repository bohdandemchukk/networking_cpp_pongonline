#include "Simulation.h"
#include "Types.h"
#include <algorithm>
#include <cmath>

static float clampf(float v, float a, float b) {
	return std::max(a, std::min(v, b));
}

static Vec2 normalize(Vec2 v) {
	const float len{ sqrt(v.x * v.x + v.y * v.y) };
	if (len > 0.00001f) {
		v.x /= len;
		v.y /= len;
	}

	return v;
}

static float paddleCenterY(const GameConfig& config, const Paddle& paddle) {
	return paddle.y + config.PADDLE_H * 0.5f;
}

static void clampPaddle(const GameConfig& config, Paddle& paddle) {
	paddle.y = clampf(paddle.y, 0.f, config.FIELD_H - config.PADDLE_H);
}

static void resetBall(const GameConfig& config, GameState& state) {
	state.ball.pos = { config.FIELD_W * 0.5f, config.FIELD_H * 0.5f };
	state.ball.vel = { 0.f, 0.f };
}

static Vec2 reflectFromPaddle(const GameConfig& config, const Paddle& paddle, const Ball& ball, bool isLeft) {
	const float offset{ ball.pos.y - paddleCenterY(config, paddle) };
	const float maxOffset{ config.PADDLE_H * 0.5f };
	const float normalizedOffset{ clampf(offset / maxOffset, -1.f, 1.f) };

	const float angle{ normalizedOffset * config.MAX_ANGLE_DEG * config.PI / 180 };

	float x{ std::cos(angle) };
	float y{ std::sin(angle) };

	if (!isLeft) x *= -1.f;

	Vec2 dir{ normalize({x, y}) };

	return { dir.x * config.BALL_SPEED, dir.y * config.BALL_SPEED };
}

static void startCountdown(const GameConfig& config, GameState& state) {
	state.phase = Phase::Countdown;
	state.countdownLeftSec = config.SERVE_DELAY_SEC;
}

static void serve(const GameConfig& config, GameState& state) {
	float x{ std::rand() % 2 == 0 ? -1.f : 1.f };
	float y{ std::rand() % 2 == 0 ? -0.5f : 0.5f };
	Vec2 dir{ normalize({x, y}) };
	state.ball.vel = { dir.x * config.BALL_SPEED, dir.y * config.BALL_SPEED };
	state.phase = Phase::Playing;
}

static bool aabb(float ax, float ay, float aw, float ah,
	float bx, float by, float bw, float bh) {
	return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}


namespace Simulation {
	void resetMatch(const GameConfig& config, GameState& state) {
		state.left = { config.PADDLE_Y, 0.f };
		state.right = { config.PADDLE_Y, 0.f };
		resetBall(config, state);
		state.score = { 0, 0 };
		state.phase = Phase::WaitingServe;
		state.winner = 0;
		state.tick = 0;
		state.countdownLeftSec = 0.f;
	}

	void step(const GameConfig& config, GameState& state, const Input& input, float dt) {
		state.left.dir = input.leftDir;
		state.right.dir = input.rightDir;

		state.left.y += state.left.dir * config.PADDLE_SPEED * dt;
		state.right.y += state.right.dir * config.PADDLE_SPEED * dt;
		clampPaddle(config, state.left);
		clampPaddle(config, state.right);

		switch (state.phase) {
		case Phase::WaitingServe:
			if (input.servePressed) startCountdown(config, state);
			break;

		case Phase::Countdown:
			
			state.countdownLeftSec -= dt;
			if (state.countdownLeftSec <= 0.f) {
				state.countdownLeftSec = 0.f;
				serve(config, state);
			}
			break;

		case Phase::Playing: {
			state.ball.pos.x += state.ball.vel.x * dt;
			state.ball.pos.y += state.ball.vel.y * dt;

			// check collision (ball with walls)
			if (state.ball.pos.y - config.BALL_RADIUS < 0.f) {
				state.ball.pos.y = config.BALL_RADIUS;
				state.ball.vel.y *= -1.f;
			}

			if (state.ball.pos.y + config.BALL_RADIUS > config.FIELD_H) {
				state.ball.pos.y = config.FIELD_H - config.BALL_RADIUS;
				state.ball.vel.y *= -1.f;
			}

			float ballAabbX{ state.ball.pos.x - config.BALL_RADIUS };
			float ballAabbY{ state.ball.pos.y - config.BALL_RADIUS };
			float ballSize{ config.BALL_RADIUS * 2.f };


			// check collision (ball with paddle)
			if (aabb(config.LEFT_PADDLE_X, state.left.y, config.PADDLE_W, config.PADDLE_H,
				ballAabbX, ballAabbY, ballSize, ballSize)) {
				state.ball.vel = reflectFromPaddle(config, state.left, state.ball, true);
				state.ball.pos.x = config.LEFT_PADDLE_X + config.PADDLE_W + config.BALL_RADIUS;
			}

			if (aabb(config.RIGHT_PADDLE_X, state.right.y, config.PADDLE_W, config.PADDLE_H,
				ballAabbX, ballAabbY, ballSize, ballSize)) {
				state.ball.vel = reflectFromPaddle(config, state.right, state.ball, false);
				state.ball.pos.x = config.RIGHT_PADDLE_X - config.BALL_RADIUS;
			}

			if (state.ball.pos.x + config.BALL_RADIUS < 0.f) {
				state.score.right++;;
				resetBall(config, state);
				state.phase = Phase::WaitingServe;
			}

			if (state.ball.pos.x - config.BALL_RADIUS > config.FIELD_W) {
				state.score.left++;
				resetBall(config, state);
				state.phase = Phase::WaitingServe;
			}

			if (state.score.left >= config.POINTS_TO_WIN) {
				state.winner = 1;
				state.phase = Phase::GameOver;

			}
			else if (state.score.right >= config.POINTS_TO_WIN) {
				state.winner = 2;
				state.phase = Phase::GameOver;
			}

			break;
		}

		case Phase::GameOver:

			if (input.servePressed) resetMatch(config, state);
			break;
		}

		state.tick++;
	}
}

