#ifndef TILE_COMPONENT
#define TILE_COMPONENT

#include <SDL2/SDL.h>

#include "asset_manager.h"
#include "entity_manager.h"
#include "../lib/glm/glm.hpp"

namespace Radix
{
	class TileComponent: public Component
	{
		public:
			glm::vec2 position;
			SDL_Texture* texture;
			SDL_Rect source_rectangle;
			SDL_Rect destination_rectangle;

			TileComponent(int, int, int, int, int, int, std::string);

			~TileComponent();

			void Update(float) override;

			void Render() override;
	};
}

#endif