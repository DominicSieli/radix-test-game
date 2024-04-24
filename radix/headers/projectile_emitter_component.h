#ifndef PROJECTILE_EMITTER_COMPONENT
#define PROJECTILE_EMITTER_COMPONENT

#include "entity_manager.h"
#include "../lib/glm/glm.hpp"
#include "transform_component.h"

namespace Radix
{
	class ProjectileEmitterComponent: public Component
	{
		private:
			bool loop;
			int speed;
			int range;
			float radian;
			glm::vec2 origin;
			TransformComponent* transform_component;

		public:
			ProjectileEmitterComponent(int, int, int, bool);

			void Initialize() override;

			void Update(float) override;
	};
}

#endif