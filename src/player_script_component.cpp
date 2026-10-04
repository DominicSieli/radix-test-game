#include <SDL3/SDL.h>

#include "player_animations.h"
#include "player_script_component.h"

#include "../radix/src/game.h"
#include "../radix/src/vector_2.h"

PlayerScriptComponent::PlayerScriptComponent()
{}


PlayerScriptComponent::~PlayerScriptComponent()
{}

void PlayerScriptComponent::initialize()
{
	this->animated_sprite_component = entity->get_component<AnimatedSpriteComponent>();
	this->transform_component = entity->get_component<TransformComponent>();
}

void PlayerScriptComponent::update(float delta_time)
{
	if(Game::input_event.type == SDL_EVENT_KEY_DOWN)
	{
		float speed = 400;

		if(Game::input_event.key.key == SDLK_UP)
		{
			this->transform_component->translate(Vector2<float>(0, -speed));
			this->animated_sprite_component->play(PLAYER_UP);
		}

		if(Game::input_event.key.key == SDLK_DOWN)
		{
			this->transform_component->translate(Vector2<float>(0, speed));
			animated_sprite_component->play(PLAYER_DOWN);
		}

		if(Game::input_event.key.key == SDLK_LEFT)
		{
			this->transform_component->translate(Vector2<float>(-speed, 0));
			animated_sprite_component->play(PLAYER_LEFT);
		}

		if(Game::input_event.key.key == SDLK_RIGHT)
		{
			this->transform_component->translate(Vector2<float>(speed, 0));
			animated_sprite_component->play(PLAYER_RIGHT);
		}

		if(Game::input_event.key.key == SDLK_SPACE)
		{}
	}

	if(Game::input_event.type == SDL_EVENT_KEY_UP)
	{
		if(Game::input_event.key.key == SDLK_UP)
		{
			this->transform_component->translate(Vector2<float>(0, 0));
		}

		if(Game::input_event.key.key == SDLK_DOWN)
		{
			this->transform_component->translate(Vector2<float>(0, 0));
		}

		if(Game::input_event.key.key == SDLK_LEFT)
		{
			this->transform_component->translate(Vector2<float>(0, 0));
		}

		if(Game::input_event.key.key == SDLK_RIGHT)
		{
			this->transform_component->translate(Vector2<float>(0, 0));
		}

		if(Game::input_event.key.key == SDLK_SPACE)
		{}
	}
}