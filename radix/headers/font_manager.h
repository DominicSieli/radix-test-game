#ifndef FONT_MANAGER
#define FONT_MANAGER

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

namespace Radix
{
	class FontManager
	{
		public:
			static TTF_Font* LoadFont(const char*, int);

			static void DrawFont(SDL_Texture*, SDL_Rect);
	};
}

#endif