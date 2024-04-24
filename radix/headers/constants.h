#ifndef CONSTANTS
#define CONSTANTS

#include <SDL2/SDL.h>

namespace Radix
{
	const unsigned int FPS = 60;
	const unsigned int LAYER_COUNT = 7;
	const unsigned int WINDOW_WIDTH = 800;
	const unsigned int WINDOW_HEIGHT = 600;
	const unsigned int FRAME_TARGET_TIME = 1000 / FPS;

	const SDL_Color GREEN = {0, 255, 0, 255};
	const SDL_Color WHITE = {255, 255, 255, 255};

	enum LayerType
	{
		TILEMAP = 0,
		VEGETATION = 1,
		ENEMY = 2,
		PLAYER = 3,
		PROJECTILE = 4,
		OBSTACLE = 5,
		UI = 6
	};

	enum CollisionType
	{
		NO_COLLISION = 0,
		PLAYER_ENEMY_COLLISION = 1,
		PLAYER_PROJECTILE_COLLISION = 2,
		ENEMY_PROJECTILE_COLLISION = 3,
		PLAYER_VEGETATION_COLLISION = 4,
		PLAYER_LEVEL_COMPLETE_COLLISION = 5
	};
}

#endif