#ifndef COLLIDER_COMPONENT
#define COLLIDER_COMPONENT

#include <SDL2/SDL.h>

#include "game.h"
#include "entity_manager.h"
#include "transform_component.h"

namespace Radix
{
	class ColliderComponent: public Component
	{
		public:
			std::string tag;
			SDL_Rect collider;
			SDL_Rect source_rectangle;
			SDL_Rect destination_rectangle;
			TransformComponent* transform_component;

			ColliderComponent(std::string, int, int, int, int);

			void Initialize() override;

			void Update(float) override;
	};
}

#endif