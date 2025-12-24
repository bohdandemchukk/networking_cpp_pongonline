#include "SfmlRenderer.h"
#include <string>

SfmlRenderer::SfmlRenderer(const GameConfig& config)
	: m_config{ config }, 
	m_font{ "assets/arial.ttf" },
	m_scoreText{ m_font, "", 32 },
	m_winText{ m_font, "", 64 },
	m_hintText{ m_font, "Press Space to Continue", 24 } 
{
	m_left.setSize({ m_config.PADDLE_W, m_config.PADDLE_H });
	m_right.setSize({ m_config.PADDLE_W, m_config.PADDLE_H });

	m_ball.setRadius(m_config.BALL_RADIUS);

	m_scoreText.setPosition({ m_config.FIELD_W * 0.5f - 30.f, 30.f });

	m_winText.setPosition({ m_config.FIELD_W * 0.5f - 130.f, 80.f });

	m_hintText.setPosition({ m_config.FIELD_W * 0.5f - 120.f, m_config.FIELD_H - 40.f });
}


void SfmlRenderer::draw(sf::RenderWindow& window, const GameState& state) {
	m_left.setPosition({ m_config.LEFT_PADDLE_X , state.left.y });
	m_right.setPosition({ m_config.RIGHT_PADDLE_X, state.right.y });
	m_ball.setPosition({ state.ball.pos.x - m_config.BALL_RADIUS, state.ball.pos.y - m_config.BALL_RADIUS });

	m_scoreText.setString(std::to_string(state.score.left) + " : " + std::to_string(state.score.right));

	m_winText.setString("");

	if (state.phase == Phase::Countdown) {
		int sec{ static_cast<int>(std::ceil(state.countdownLeftSec)) };
		m_winText.setString(std::to_string(sec));
	}
	else if (state.phase == Phase::GameOver) {
		m_winText.setString(state.winner == 1 ? "LEFT WIN" : "RIGHT WIN");
	}

	window.draw(m_scoreText);
	if (!m_winText.getString().isEmpty()) window.draw(m_winText);

	window.draw(m_left);
	window.draw(m_right);
	window.draw(m_ball);

	if (state.phase == Phase::GameOver) window.draw(m_hintText);


}