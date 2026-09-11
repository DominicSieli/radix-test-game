#ifndef CONTROLS_COMPONENT_H
#define CONTROLS_COMPONENT_H

#include <SDL3/SDL.h>

#include "../radix/src/sprite_component.h"
#include "../radix/src/transform_component.h"

namespace radix
{
	class ControlsComponent: public Component
	{
		public:
			SDL_Event* event;
			SpriteComponent* sprite_component;
			TransformComponent* transform_component;

			ControlsComponent();

			ControlsComponent(SDL_Event*);

			void initialize() override;

			void update(float) override;
	};
}

#endif