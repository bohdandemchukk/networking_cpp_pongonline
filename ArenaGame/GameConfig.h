#ifndef GAMECONFIG_H
#define GAMECONFIG_H

struct GameConfig {
	float FIELD_W{ 800.f };
	float FIELD_H{ 600.f };

	float PADDLE_W{ 10.f };
	float PADDLE_H{ 100.f };
	float PADDLE_SPEED{ 300.f };
	
	float LEFT_PADDLE_X{ 50.f };
	float RIGHT_PADDLE_X{ 740.f };
	float PADDLE_Y{ 250.f };

	float BALL_RADIUS{ 10.f };
	float BALL_SPEED{ 400.f };

	int POINTS_TO_WIN{ 5 };

	float SERVE_DELAY_SEC{ 3.f };

	float MAX_ANGLE_DEG{ 60.f };
	float PI{ 3.14f };

};

#endif // GAMECONFIG_H