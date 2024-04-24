#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

namespace Radix
{
	class TextureManager
	{
		public:
			static SDL_Texture* LoadTexture(const char*);

			static void Draw(SDL_Texture*, SDL_Rect, SDL_Rect, SDL_RendererFlip);
	};
}

#endif