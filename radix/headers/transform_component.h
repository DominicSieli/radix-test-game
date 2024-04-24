#ifndef TRANSFORM_COMPONENT
#define TRANSFORM_COMPONENT

#include <SDL2/SDL.h>

#include "game.h"
#include "entity_manager.h"
#include "../lib/glm/glm.hpp"

namespace Radix
{
	class TransformComponent: public Component
	{
		public:
			int scale;
			glm::vec2 position;
			glm::vec2 velocity;
			glm::vec2 dimension;

			TransformComponent(int, int, int, int, int, int, int);

			void Initialize() override;

			void Update(float) override;

			void Render() override;
	};
}

#endif