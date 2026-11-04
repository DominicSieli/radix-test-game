#include <SDL3/SDL.h>

#include "player.h"
#include "texture_id.h"
#include "collision_tags.h"
#include "entity_type_id.h"
#include "player_animations.h"
#include "player_script_component.h"

#include "../radix/src/game.h"
#include "../radix/src/animation.h"
#include "../radix/src/collider_component.h"
#include "../radix/src/animated_sprite_component.h"

Entity* create_player(const Vector2<float>& position)
{
	Entity* player = Game::entity_manager.create_entity(PLAYER_ID, 2);

	const char* const CHOPPER_PNG_PATH = "./assets/images/chopper-spritesheet.png";

	Game::asset_manager.add_texture(CHOPPER_TEXTURE_ID, CHOPPER_PNG_PATH);

	std::unordered_map<unsigned int, Animation> chopper_animations;

	Animation up = Animation(3, 2, 10);
	Animation down = Animation(0, 2, 10);
	Animation left = Animation(2, 2, 10);
	Animation right = Animation(1, 2, 10);

	chopper_animations.try_emplace(PLAYER_UP, up);
	chopper_animations.try_emplace(PLAYER_DOWN, down);
	chopper_animations.try_emplace(PLAYER_LEFT, left);
	chopper_animations.try_emplace(PLAYER_RIGHT, right);

	player->add_component<TransformComponent>(Vector2<float>(32, 32), Vector2<float>(position.x, position.y), Vector2<float>(1, 1), 0);
	player->add_component<AnimatedSpriteComponent>(false, SDL_FLIP_NONE, CHOPPER_TEXTURE_ID, PLAYER_DOWN, chopper_animations);
	player->add_component<PlayerScriptComponent>();
	player->add_component<ColliderComponent>(PLAYER_COLLIDER_TAG, Vector2<float>(32, 32));

	return player;
}
