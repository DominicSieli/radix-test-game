#include "controls_component.h"

#include <SDL3/SDL.h>

ControlsComponent::ControlsComponent()
{}

ControlsComponent::ControlsComponent(SDL_Event* event): event{event}
{}

void ControlsComponent::initialize()
{
	this->animated_sprite_component = entity->get_component<AnimatedSpriteComponent>();
	this->transform_component = entity->get_component<TransformComponent>();
}

void ControlsComponent::update(float delta_time)
{
	if(this->event->type == SDL_EVENT_KEY_DOWN)
	{
		int speed = 100;

		if(this->event->key.key == SDLK_UP)
		{
			this->transform_component->velocity.x = 0;
			this->transform_component->velocity.y = -speed;
			this->animated_sprite_component->play("Up");
		}

		if(this->event->key.key == SDLK_DOWN)
		{
			transform_component->velocity.x = 0;
			transform_component->velocity.y = speed;
			animated_sprite_component->play("Down");
		}

		if(this->event->key.key == SDLK_LEFT)
		{
			transform_component->velocity.x = -speed;
			transform_component->velocity.y = 0;
			animated_sprite_component->play("Left");
		}

		if(this->event->key.key == SDLK_RIGHT)
		{
			transform_component->velocity.x = speed;
			transform_component->velocity.y = 0;
			animated_sprite_component->play("Right");
		}

		if(this->event->key.key == SDLK_SPACE)
		{}
	}

	if(this->event->type == SDL_EVENT_KEY_UP)
	{
		if(this->event->key.key == SDLK_UP)
		{
			transform_component->velocity.y = 0;
		}

		if(this->event->key.key == SDLK_DOWN)
		{
			transform_component->velocity.y = 0;
		}

		if(this->event->key.key == SDLK_LEFT)
		{
			transform_component->velocity.x = 0;
		}

		if(this->event->key.key == SDLK_RIGHT)
		{
			transform_component->velocity.x = 0;
		}

		if(this->event->key.key == SDLK_SPACE)
		{}
	}
}