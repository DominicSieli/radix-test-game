#ifndef CONTROLS_COMPONENT_H
#define CONTROLS_COMPONENT_H

#include <SDL3/SDL.h>

#include "../radix/src/transform_component.h"
#include "../radix/src/animated_sprite_component.h"

using namespace Radix;

class PlayerScriptComponent: public Component
{
	public:
		TransformComponent* transform_component;
		AnimatedSpriteComponent* animated_sprite_component;

		PlayerScriptComponent();

		~PlayerScriptComponent();

		void initialize() override;

		void update(const float&) override;
};

#endif