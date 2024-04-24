#ifndef ENTITY
#define ENTITY

#include <map>
#include <vector>
#include <string>
#include <typeinfo>

#include "constants.h"
#include "component.h"
#include "entity_manager.h"

namespace Radix
{
	class EntityManager;

	class Entity
	{
		private:
			bool active;
			EntityManager& entity_manager;
			std::vector<Component*> components;
			std::map<const std::type_info*, Component*> component_type_map;

		public:
			LayerType layer;
			std::string name;

			Entity(EntityManager&);

			Entity(EntityManager&, std::string, LayerType);

			void Update(float);

			void Render();

			void Destroy();

			bool Active() const;

			void ListComponents() const;

			template<typename T, typename... T_ARGS>
				T& AddComponent(T_ARGS&&... args)
				{
					T* component(new T(std::forward<T_ARGS>(args)...));
					component->entity = this;
					components.emplace_back(component);
					component_type_map[&typeid(*component)] = component;
					component->Initialize();
					return *component;
				}

			template<typename T>
				T* GetComponent()
				{
					return static_cast<T*>(component_type_map[&typeid(T)]);
				}

			template<typename T>
				bool HasComponent() const
				{
					return component_type_map.count(&typeid(T));
				}
	};
}

#endif