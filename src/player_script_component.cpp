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
	float speed = 200.00f;

	if(Game::input_manager.keyboard_state[SDL_SCANCODE_W] || Game::input_manager.keyboard_state[SDL_SCANCODE_UP])
	{
		if(!Game::state_manager.is_paused())
		{
			transform_component->translate(Vector2<float>(0.00f, -speed));
			animated_sprite_component->play(PLAYER_UP);
		}
	}

	if(Game::input_manager.keyboard_state[SDL_SCANCODE_S] || Game::input_manager.keyboard_state[SDL_SCANCODE_DOWN])
	{
		if(!Game::state_manager.is_paused())
		{
			transform_component->translate(Vector2<float>(0.00f, speed));
			animated_sprite_component->play(PLAYER_DOWN);
		}
	}

	if(Game::input_manager.keyboard_state[SDL_SCANCODE_A] || Game::input_manager.keyboard_state[SDL_SCANCODE_LEFT])
	{
		if(!Game::state_manager.is_paused())
		{
			transform_component->translate(Vector2<float>(-speed, 0.00f));
			animated_sprite_component->play(PLAYER_LEFT);
		}
	}

	if(Game::input_manager.keyboard_state[SDL_SCANCODE_D] || Game::input_manager.keyboard_state[SDL_SCANCODE_RIGHT])
	{
		if(!Game::state_manager.is_paused())
		{
			transform_component->translate(Vector2<float>(speed, 0.00f));
			animated_sprite_component->play(PLAYER_RIGHT);
		}
	}

	if(Game::input_manager.get_event().key.key == SDLK_SPACE)
	{}

	constrain_to_camera_bounds(get_entity());
}