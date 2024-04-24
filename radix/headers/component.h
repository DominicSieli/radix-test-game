#ifndef COMPONENT
#define COMPONENT

#include "entity.h"

namespace Radix
{
	class Entity;

	class Component
	{
		public:
			Entity* entity;

			virtual ~Component() {}

			virtual void Initialize() {}

			virtual void Update(float) {}

			virtual void Render() {}
	};
}

#endif