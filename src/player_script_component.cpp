#include <SDL3/SDL.h>

#include "player_animations.h"
#include "player_script_component.h"

#include "../radix/src/game.h"
#include "../radix/src/vectors.h"
#include "../radix/src/constraints.h"

PlayerScriptComponent::PlayerScriptComponent()
{}

PlayerScriptComponent::~PlayerScriptComponent()
{}

void PlayerScriptComponent::initialize()
{
	animated_sprite_component = get_entity()->get_component<AnimatedSpriteComponent>();
	transform_component = get_entity()->get_component<TransformComponent>();
}

void PlayerScriptComponent::update(const float&)
{
	if(Game::input_manager.get_event().type == SDL_EVENT_KEY_DOWN)
	{
		float speed = 400.00f;

		if(!Game::state_manager.is_paused())
		{
			if(Game::input_manager.get_event().key.key == SDLK_UP)
			{
				transform_component->translate(Vector2<float>(0.00f, -speed));
				animated_sprite_component->play(PLAYER_UP);
			}

			if(Game::input_manager.get_event().key.key == SDLK_DOWN)
			{
				transform_component->translate(Vector2<float>(0.00f, speed));
				animated_sprite_component->play(PLAYER_DOWN);
			}

			if(Game::input_manager.get_event().key.key == SDLK_LEFT)
			{
				transform_component->translate(Vector2<float>(-speed, 0.00f));
				animated_sprite_component->play(PLAYER_LEFT);
			}

			if(Game::input_manager.get_event().key.key == SDLK_RIGHT)
			{
				transform_component->translate(Vector2<float>(speed, 0.00f));
				animated_sprite_component->play(PLAYER_RIGHT);
			}
		}

		if(Game::input_manager.get_event().key.key == SDLK_SPACE)
		{}
	}

	constrain_to_camera_bounds(get_entity());
}