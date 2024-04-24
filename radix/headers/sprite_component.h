#ifndef SPRITE_COMPONENT
#define SPRITE_COMPONENT

#include <string>
#include <SDL2/SDL.h>

#include "animation.h"
#include "asset_manager.h"
#include "texture_manager.h"
#include "transform_component.h"

namespace Radix
{
	class SpriteComponent: public Component
	{
		private:
			int speed;
			int frames;
			bool fixed;
			bool animated;
			SDL_Rect source;
			SDL_Texture* texture;
			SDL_Rect destination;
			unsigned int index = 0;
			std::string animation_name;
			TransformComponent* transform_component;
			std::map<std::string, Animation> animations;

		public:
			SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;

			SpriteComponent(const char*);

			SpriteComponent(std::string, unsigned int, unsigned int, bool, bool);

			void Play(std::string);

			void SetTexture(std::string);

			void Initialize() override;

			void Update(float) override;

			void Render() override;
	};
}

#endif