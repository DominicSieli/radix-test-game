#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <vector>

#include "entity.h"
#include "component.h"

namespace Radix
{
	class EntityManager
	{
		private:
			std::vector<Entity*> entities;

		public:
			void Clear();

			void Update(float);

			void Render();

			bool IsEmpty() const;

			void ListEntities() const;

			Entity& AddEntity(std::string, LayerType);

			std::vector<Entity*> GetEntities() const;

			std::vector<Entity*> GetEntitiesByLayer(LayerType) const;

			CollisionType CheckCollisions() const;

			void DestroyInactiveEntities();

			unsigned int EntityCount();
	};
}

#endif