#include <string>
#include <vector>
#include <iostream>
#include <SDL3/SDL.h>

#include "player.h"
#include "colors.h"
#include "font_id.h"
#include "settings.h"
#include "texture_id.h"
#include "asset_paths.h"
#include "collision_tags.h"
#include "entity_type_id.h"

#include "../radix/src/game.h"
#include "../radix/src/entity.h"
#include "../radix/src/vectors.h"
#include "../radix/src/tile_map.h"
#include "../radix/src/component.h"
#include "../radix/src/animation.h"
#include "../radix/src/collision.h"
#include "../radix/src/time_manager.h"
#include "../radix/src/audio_manager.h"
#include "../radix/src/asset_manager.h"
#include "../radix/src/window_manager.h"
#include "../radix/src/text_component.h"
#include "../radix/src/entity_manager.h"
#include "../radix/src/render_manager.h"
#include "../radix/src/collision_manager.h"
#include "../radix/src/collider_component.h"
#include "../radix/src/transform_component.h"
#include "../radix/src/static_sprite_component.h"
#include "../radix/src/animated_sprite_component.h"

using namespace Radix;

Entity* player;
TileMap* tile_map;
Entity* fps_display;
const unsigned int render_layer_count = 10;

using namespace Radix;

Game::Game()
{
	if(!window_manager.initialize(RESOLUTION_X, RESOLUTION_Y))
	{
		SDL_Log("Window initialization failed.");
		destroy();
	}

	camera_manager.initialize(0, 0, RESOLUTION_X, RESOLUTION_Y);

	if(!render_manager.initialize(window_manager.get_window(), nullptr, 0))
	{
		SDL_Log("Renderer initialization failed.");
		destroy();
	}

	SDL_Log("graphics api: %s", SDL_GetRendererName(render_manager.get_renderer()));

	load_level(0);

	if(!audio_manager.initialize())
	{
		SDL_Log("Audio initialization failed.");
		destroy();
	}

	audio_manager.load_audio("./assets/music/gametrack-4.wav", 1);
	audio_manager.play_music(1);

	return;
}

Game::~Game()
{}

void Game::load_level(const unsigned int& level_number)
{
	asset_manager.add_font(CHARRIOT_ID, CHARRIOT_TTF_PATH, 24);
	asset_manager.add_texture(TANK_TEXTURE_ID, TANK_PNG_PATH);
	asset_manager.add_texture(RADAR_TEXTURE_ID, RADAR_PNG_PATH);
	asset_manager.add_texture(HELIPORT_TEXTURE_ID, HELIPORT_PNG_PATH);
	asset_manager.add_texture(JUNGLE_MAP_TEXTURE_ID, JUNGLE_PNG_PATH);
	asset_manager.add_texture(ENEMY_BULLET_TEXTURE_ID, BULLET_PNG_PATH);

	player = create_player(Vector2<float>(240, 110));

	tile_map = new TileMap(JUNGLE_MAP_TEXTURE_ID, Vector2<int>(25, 20), 32, 2);
	tile_map->load_map(JUNGLE_MAP_PATH, TILE_ID, 0);

	std::string fps_text = "FPS: " + std::to_string(time_manager.get_fps());
	fps_display = entity_manager.create_entity(HUD_ID, 9);
	fps_display->add_component<TextComponent>(Vector2<float>(10, 10), fps_text, CHARRIOT_ID, WHITE);

	Entity* tank = entity_manager.create_entity(TANK_ID, 1);
	tank->add_component<TransformComponent>(Vector2<float>(32, 32), Vector2<float>(250, 495), Vector2<float>(1, 1), 0);
	tank->add_component<StaticSpriteComponent>(false, SDL_FLIP_NONE, TANK_TEXTURE_ID);
	tank->add_component<ColliderComponent>(ENEMY_COLLIDER_TAG, Vector2<float>(32, 32));
	TransformComponent* tank_transform = tank->get_component<TransformComponent>();
	tank_transform->translate(Vector2<float>(10000, 0));

	Entity* bullet = entity_manager.create_entity(ENEMY_BULLET_ID, 1);
	bullet->add_component<TransformComponent>(Vector2<float>(4, 4), Vector2<float>(tank_transform->get_position().x+16, tank_transform->get_position().y+16), Vector2<float>(1, 1), 0);
	bullet->add_component<StaticSpriteComponent>(false, SDL_FLIP_NONE, ENEMY_BULLET_TEXTURE_ID);
	bullet->add_component<ColliderComponent>(ENEMY_BULLET_COLLIDER_TAG, Vector2<float>(4, 4));
	TransformComponent* bullet_transform = bullet->get_component<TransformComponent>();
	bullet_transform->translate(Vector2<float>(2000, 0));

	Entity* heliport = entity_manager.create_entity(HELIPORT_ID, 1);
	heliport->add_component<TransformComponent>(Vector2<float>(32, 32), Vector2<float>(470, 420), Vector2<float>(1, 1), 0);
	heliport->add_component<StaticSpriteComponent>(false, SDL_FLIP_NONE, HELIPORT_TEXTURE_ID);
	heliport->add_component<ColliderComponent>(HELIPORT_COLLIDER_TAG, Vector2<float>(32, 32));

	std::unordered_map<unsigned int, Animation> radar_animation;

	Animation rotate_radar = Animation(0, 8, 150);
	radar_animation.emplace(0, rotate_radar);

	Entity* radar = entity_manager.create_entity(RADAR_ID, 9);
	radar->add_component<TransformComponent>(Vector2<float>(64, 64), Vector2<float>(720, 15), Vector2<float>(1, 1), 0);
	radar->add_component<AnimatedSpriteComponent>(true, SDL_FLIP_NONE, RADAR_TEXTURE_ID, 0, radar_animation);
}

void Game::input()
{
	input_manager.poll();

	switch(input_manager.get_event().type)
	{
		case SDL_EVENT_QUIT: { state_manager.exit_game(); break; }
		case SDL_EVENT_KEY_DOWN: { if(input_manager.get_event().key.key == SDLK_ESCAPE) state_manager.exit_game(); break; }
		default: break;
	}
}

void Game::update()
{
	Game::time_manager.cap_fps(TARGET_FPS);
	Game::time_manager.calculate_fps();
	Game::time_manager.calculate_delta_time();

	fps_display->get_component<TextComponent>()->set_text("FPS: " + std::to_string(Game::time_manager.get_fps()), CHARRIOT_ID);

	entity_manager.update(Game::time_manager.get_delta_time());
	update_camera_movement();
	check_collisions();

	if(Game::input_manager.get_event().type == SDL_EVENT_KEY_DOWN)
	{
		if(Game::input_manager.get_event().key.key == SDLK_SPACE)
		{
			if(Game::state_manager.is_paused())
			{
				Game::state_manager.unpause();
			}
			else if(!Game::state_manager.is_paused())
			{
				Game::state_manager.pause();
			}
		}
	}

	if(Game::state_manager.is_paused())
	{
		player->get_component<TransformComponent>()->set_inactive();
		player->get_component<AnimatedSpriteComponent>()->set_inactive();
	}

	if(!Game::state_manager.is_paused())
	{

		player->get_component<TransformComponent>()->set_active();
		player->get_component<AnimatedSpriteComponent>()->set_active();
	}
}

void Game::render()
{
	render_manager.render(render_layer_count);
}

void Game::update_camera_movement()
{
	float tile_map_x = static_cast<float>(tile_map->get_map_dimensions().x);
	float tile_map_y = static_cast<float>(tile_map->get_map_dimensions().y);
	TransformComponent* player_transform = player->get_component<TransformComponent>();

	camera_manager.update_position(player_transform->get_position(), Vector2<float>(tile_map_x, tile_map_y));
}

void Game::check_collisions()
{
	std::vector<Collision> collisions;

	Collision player_enemy_collision = Collision(PLAYER_COLLIDER_TAG, ENEMY_COLLIDER_TAG, PLAYER_ENEMY_COLLISION);
	Collision player_heliport_collision = Collision(PLAYER_COLLIDER_TAG, HELIPORT_COLLIDER_TAG, PLAYER_HELIPORT_COLLISION);
	Collision player_enemy_projectile_collision = Collision(PLAYER_COLLIDER_TAG, ENEMY_BULLET_COLLIDER_TAG, PLAYER_ENEMY_BULLET_COLLISION);

	collisions.push_back(player_enemy_collision);
	collisions.push_back(player_heliport_collision);
	collisions.push_back(player_enemy_projectile_collision);

	unsigned int collision_type = collision_manager.check_collisions(collisions, NO_COLLISION);

	if(collision_type == PLAYER_ENEMY_COLLISION)
	{
		std::cout << "PLAYER_ENEMY_COLLISION\n";
		process_gameover();
	}

	if(collision_type == PLAYER_ENEMY_BULLET_COLLISION)
	{
		std::cout << "PLAYER_ENEMY_BULLET_COLLISION\n";
		process_gameover();
	}

	if(collision_type == PLAYER_HELIPORT_COLLISION)
	{
		std::cout << "PLAYER_HELIPORT_COLLISION\n";
		process_next_level(1);
	}
}

void Game::process_gameover()
{
	std::cout << "Game Over\n";
	state_manager.exit_game();
}

void Game::process_next_level(const unsigned int& level_number)
{
	std::cout << "Next Level\n";
	state_manager.exit_game();
}

void Game::destroy()
{
	SDL_DestroyWindow(window_manager.get_window());
	SDL_Quit();
}