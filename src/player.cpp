#include "player.h"
#include "texture_id.h"
#include "collision_tags.h"
#include "player_animations.h"
#include "player_script_component.h"

#include "../radix/src/game.h"
#include "../radix/src/animation.h"
#include "../radix/src/collider_component.h"
#include "../radix/src/animated_sprite_component.h"

Entity* create_player(const Vector2<float>& position)
{
	Entity* player = Game::entity_manager.add_entity("player", 2);

	const char* CHOPPER_PNG_PATH = "./assets/images/chopper-spritesheet.png";

	Game::asset_manager.add_texture(CHOPPER_TEXTURE_ID, CHOPPER_PNG_PATH);

	std::map<unsigned int, Animation> chopper_animations;

	Animation player_up = Animation(3, 2, 9);
	Animation player_down = Animation(0, 2, 9);
	Animation player_left = Animation(2, 2, 9);
	Animation player_right = Animation(1, 2, 9);

	chopper_animations.emplace(PLAYER_UP, player_up);
	chopper_animations.emplace(PLAYER_DOWN, player_down);
	chopper_animations.emplace(PLAYER_LEFT, player_left);
	chopper_animations.emplace(PLAYER_RIGHT, player_right);

	player->add_component<TransformComponent>(Vector2<float>(position.x, position.y), Vector2<float>(1, 1), 0);
	player->add_component<AnimatedSpriteComponent>(chopper_animations, CHOPPER_TEXTURE_ID, PLAYER_DOWN, Vector2<int>(32, 32), false);
	player->add_component<PlayerScriptComponent>();
	player->add_component<ColliderComponent>(PLAYER_COLLIDER_TAG, Vector2<int>(32, 32));

	return player;
}
