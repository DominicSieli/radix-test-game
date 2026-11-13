#ifndef PLAYER_SCRIPT_COMPONENT_H
#define PLAYER_SCRIPT_COMPONENT_H

#include <SDL3/SDL.h>

#include "../radix/src/transform_component.h"
#include "../radix/src/sprite_component.h"

using namespace Radix;

class PlayerScriptComponent: public Component
{
	public:
		SpriteComponent* sprite_component;
		TransformComponent* transform_component;

		PlayerScriptComponent();

		~PlayerScriptComponent();

		void initialize() override;

		void update(const float&) override;
};

#endif