#ifndef ACTIONS_COMPONENT_H
#define ACTIONS_COMPONENT_H

#include <SDL3/SDL.h>

#include "../radix/src/sprite_component.h"
#include "../radix/src/transform_component.h"

namespace radix
{
	class ActionsComponent: public Component
	{
		public:
			SDL_Event* event;
			SpriteComponent* sprite_component;
			TransformComponent* transform_component;

			ActionsComponent();

			ActionsComponent(SDL_Event*);

			void initialize() override;

			void update(float) override;
	};
}

#endif