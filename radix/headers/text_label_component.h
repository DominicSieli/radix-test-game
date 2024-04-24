#ifndef TEXT_LABEL_COMPONENT
#define TEXT_LABEL_COMPONENT

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"
#include "font_manager.h"
#include "asset_manager.h"
#include "entity_manager.h"

namespace Radix
{
	class TextLabelComponent: public Component
	{
		private:
			SDL_Color color;
			std::string text;
			SDL_Rect position;
			SDL_Texture* texture;
			std::string font_family;

		public:
			TextLabelComponent(int, int, std::string, std::string, const SDL_Color&);

			void SetLabelText(std::string, std::string);

			void Render() override;
	};
}

#endif