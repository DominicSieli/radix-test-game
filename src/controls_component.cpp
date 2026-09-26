#include <SDL3/SDL.h>

#include "../radix/src/game.h"
#include "player_animations.h"
#include "controls_component.h"
#include "../radix/src/vector_2.h"

ControlsComponent::ControlsComponent()
{}

ControlsComponent::ControlsComponent(SDL_Event* event): event{event}
{}

void ControlsComponent::initialize()
{
	this->animated_sprite_component = entity->get_component<AnimatedSpriteComponent>();
	this->transform_component = entity->get_component<TransformComponent>();
}

void ControlsComponent::update(double delta_time)
{
	if(this->event->type == SDL_EVENT_KEY_DOWN)
	{
		unsigned int speed = 200 * Game::delta_time;

		if(this->event->key.key == SDLK_UP)
		{
			this->transform_component->translate(Vector2<unsigned int>(0, -speed));
			this->animated_sprite_component->play(PLAYER_UP);
		}

		if(this->event->key.key == SDLK_DOWN)
		{
			this->transform_component->translate(Vector2<unsigned int>(0, speed));
			animated_sprite_component->play(PLAYER_DOWN);
		}

		if(this->event->key.key == SDLK_LEFT)
		{
			this->transform_component->translate(Vector2<unsigned int>(-speed, 0));
			animated_sprite_component->play(PLAYER_LEFT);
		}

		if(this->event->key.key == SDLK_RIGHT)
		{
			this->transform_component->translate(Vector2<unsigned int>(speed, 0));
			animated_sprite_component->play(PLAYER_RIGHT);
		}

		if(this->event->key.key == SDLK_SPACE)
		{}
	}

	if(this->event->type == SDL_EVENT_KEY_UP)
	{
		if(this->event->key.key == SDLK_UP)
		{
			this->transform_component->translate(Vector2<unsigned int>(0, 0));
		}

		if(this->event->key.key == SDLK_DOWN)
		{
			this->transform_component->translate(Vector2<unsigned int>(0, 0));
		}

		if(this->event->key.key == SDLK_LEFT)
		{
			this->transform_component->translate(Vector2<unsigned int>(0, 0));
		}

		if(this->event->key.key == SDLK_RIGHT)
		{
			this->transform_component->translate(Vector2<unsigned int>(0, 0));
		}

		if(this->event->key.key == SDLK_SPACE)
		{}
	}
}