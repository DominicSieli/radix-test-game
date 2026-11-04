#include "game.h"
#include "spawn_component.h"

namespace Radix
{
	SpawnComponent::SpawnComponent(const unsigned int& type_id, const unsigned int& render_layer):
		type_id{type_id}, render_layer{render_layer}
	{}

	void SpawnComponent::initialize()
	{}

	void SpawnComponent::spawn()
	{
		Entity* entity = Game::entity_manager.create_entity(type_id, render_layer);
	}
}
