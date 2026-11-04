#ifndef SPAWN_COMPONENT_H
#define SPAWN_COMPONENT_H

#include "entity.h"

namespace Radix
{
	class SpawnComponent: public Component
	{
		private:
			unsigned int type_id;
			unsigned int render_layer;

		public:
			SpawnComponent(const unsigned int&, const unsigned int&);

			void initialize() override;

			void spawn();
	};
}

#endif
