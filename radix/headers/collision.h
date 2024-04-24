#ifndef COLLISION
#define COLLISION

#include <SDL2/SDL.h>

namespace Radix
{
	class Collision
	{
		public:
			static bool CheckRectangleCollision(const SDL_Rect&, const SDL_Rect&);
	};
}

#endif