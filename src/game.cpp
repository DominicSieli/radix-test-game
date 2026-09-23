#include <vector>
#include <iostream>
#include <SDL3/SDL.h>

#include "settings.h"
#include "controls_component.h"

#include "colors.h"
#include "font_id.h"
#include "texture_id.h"
#include "asset_paths.h"
#include "collision_tags.h"
#include "player_animations.h"
#include "../radix/src/game.h"
#include "../radix/src/entity.h"
#include "../radix/src/tile_map.h"
#include "../radix/src/component.h"
#include "../radix/src/animation.h"
#include "../radix/src/collision.h"
#include "../radix/src/asset_manager.h"
#include "../radix/src/text_component.h"
#include "../radix/src/entity_manager.h"
#include "../radix/src/render_manager.h"
#include "../radix/src/spawner_component.h"
#include "../radix/src/collision_manager.h"
#include "../radix/src/collider_component.h"
#include "../radix/src/transform_component.h"
#include "../radix/src/static_sprite_component.h"
#include "../radix/src/animated_sprite_component.h"

using namespace Radix;

TileMap* tile_map;
SDL_Event Game::input_event;
SDL_Renderer* Game::renderer;
EntityManager Game::entity_manager;
AssetManager Game::asset_manager;
RenderManager Game::render_manager;
CollisionManager Game::collision_manager;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

Game::Game()
{
	if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		std::cerr << "Error Initializing SDL\n";
		return;
	}

	if(!TTF_Init())
	{
		std::cerr << "Error Initializing SDL TTF\n";
		return;
	}

	this->window = SDL_CreateWindow(NULL, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);

	if(this->window == nullptr)
	{
		std::cerr << "Error Creating SDL window\n";
		return;
	}

	SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	this->renderer = SDL_CreateRenderer(this->window, nullptr);

	if(this->renderer == nullptr)
	{
		std::cerr << "Error Creating SDL Renderer";
		return;
	}

	load_level(0);

	this->running = true;
	return;
}

Game::~Game()
{}

bool Game::is_running()
{
	return this->running;
}

Entity* player(Game::entity_manager.add_entity("player", 2));

void Game::load_level(int level_number)
{
	asset_manager.add_font(CHARRIOT_ID, CHARRIOT_TTF_PATH, 24);
	asset_manager.add_texture(TANK_TEXTURE_ID, TANK_PNG_PATH);
	asset_manager.add_texture(CHOPPER_TEXTURE_ID, CHOPPER_PNG_PATH);
	asset_manager.add_texture(RADAR_TEXTURE_ID, RADAR_PNG_PATH);
	asset_manager.add_texture(HELIPORT_TEXTURE_ID, HELIPORT_PNG_PATH);
	asset_manager.add_texture(JUNGLE_MAP_TEXTURE_ID, JUNGLE_PNG_PATH);
	asset_manager.add_texture(ENEMY_BULLET_TEXTURE_ID, BULLET_PNG_PATH);

	tile_map = new TileMap(JUNGLE_MAP_TEXTURE_ID, 2, 32);
	tile_map->load_map(JUNGLE_MAP_PATH, 25, 20, "tile", 0);

	Entity* level_name(entity_manager.add_entity("LabelLevelName", 9));
	level_name->add_component<TextComponent>(10, 10, "Level: 1", CHARRIOT_ID, WHITE);

	std::map<unsigned int, Animation> chopper_animations;

	Animation up = Animation(3, 2, 9);
	Animation down = Animation(0, 2, 9);
	Animation left = Animation(2, 2, 9);
	Animation right = Animation(1, 2, 9);

	chopper_animations.emplace(PLAYER_UP, up);
	chopper_animations.emplace(PLAYER_DOWN, down);
	chopper_animations.emplace(PLAYER_LEFT, left);
	chopper_animations.emplace(PLAYER_RIGHT, right);

	player->add_component<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
	player->add_component<AnimatedSpriteComponent>(CHOPPER_TEXTURE_ID, chopper_animations, PLAYER_DOWN, false);
	player->add_component<ControlsComponent>(&input_event);
	player->add_component<ColliderComponent>(PLAYER_COLLIDER_TAG, 240, 106, 32, 32);

	Entity* tank(entity_manager.add_entity("tank", 1));
	tank->add_component<TransformComponent>(250, 495, 5, 0, 32, 32, 1);
	tank->add_component<StaticSpriteComponent>(TANK_TEXTURE_ID, false);
	tank->add_component<ColliderComponent>(ENEMY_COLLIDER_TAG, 150, 495, 32, 32);

	TransformComponent* tank_transform = tank->get_component<TransformComponent>();
	Entity* projectile(entity_manager.add_entity("projectile", 1));
	projectile->add_component<TransformComponent>(tank_transform->position.x+16, tank_transform->position.y+16, 0, 0, 4, 4, 1);
	projectile->add_component<StaticSpriteComponent>(ENEMY_BULLET_TEXTURE_ID, false);
	projectile->add_component<ColliderComponent>(ENEMY_BULLET_COLLIDER_TAG, tank_transform->position.x+16, tank_transform->position.y+16, 4, 4);
	projectile->add_component<SpawnerComponent>(50, 0, 200, true);

	Entity* heliport(entity_manager.add_entity("heliport", 1));
	heliport->add_component<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
	heliport->add_component<StaticSpriteComponent>(HELIPORT_TEXTURE_ID, false);
	heliport->add_component<ColliderComponent>(HELIPORT_COLLIDER_TAG, 470, 420, 32, 32);

	std::map<unsigned int, Animation> radar_animation;
	Animation rotate = Animation(0, 8, 150);
	radar_animation.emplace(0, rotate);

	Entity* radar(entity_manager.add_entity("radar", 9));
	radar->add_component<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
	radar->add_component<AnimatedSpriteComponent>(RADAR_TEXTURE_ID, radar_animation, 0, true);
}

void Game::input()
{
	SDL_PollEvent(&input_event);

	switch(input_event.type)
	{
		case SDL_EVENT_QUIT: { this->running = false; break; }
		case SDL_EVENT_KEY_DOWN: { if(input_event.key.key == SDLK_ESCAPE) this->running = false; break; }
		default: break;
	}
}

void Game::update()
{
	float delta_time = (SDL_GetTicks() - ticks_last_frame) / 1000.00f;

	delta_time = (delta_time > 0.05f) ? 0.05f : delta_time;

	this->ticks_last_frame = SDL_GetTicks();

	entity_manager.update(delta_time);

	update_camera_movement();
	check_collisions();
}

void Game::render()
{
	SDL_SetRenderDrawColor(this->renderer, 21, 21, 21, 255);
	SDL_RenderClear(this->renderer);

	if(entity_manager.is_empty() == true)
	{
		return;
	}

	render_manager.render();

	SDL_RenderPresent(this->renderer);
}

void Game::update_camera_movement()
{
	TransformComponent* player_transform = player->get_component<TransformComponent>();

	camera.x = player_transform->position.x - static_cast<int>(WINDOW_WIDTH / 2);
	camera.y = player_transform->position.y - static_cast<int>(WINDOW_HEIGHT / 2);

	camera.x = (camera.x < 0) ? 0 : camera.x;
	camera.y = (camera.y < 0) ? 0 : camera.y;
	camera.x = (camera.x > camera.w) ? camera.w : camera.x;
	camera.y = (camera.y > camera.h) ? camera.h : camera.y;
}

void Game::check_collisions()
{
	Collision player_enemy_collision = {PLAYER_COLLIDER_TAG, ENEMY_COLLIDER_TAG, PLAYER_ENEMY_COLLISION};
	Collision player_heliport_collision = {PLAYER_COLLIDER_TAG, HELIPORT_COLLIDER_TAG, PLAYER_HELIPORT_COLLISION};
	Collision player_enemy_projectile_collision = {PLAYER_COLLIDER_TAG, ENEMY_BULLET_COLLIDER_TAG, PLAYER_ENEMY_BULLET_COLLISION};
	std::vector<Collision> collisions;

	collisions.push_back(player_enemy_collision);
	collisions.push_back(player_heliport_collision);
	collisions.push_back(player_enemy_projectile_collision);

	unsigned int collision_type = collision_manager.check_collisions(collisions, NO_COLLISION);

	if(collision_type == PLAYER_ENEMY_COLLISION)
	{
		std::cout << "PLAYER_ENEMY_COLLISION" << std::endl;
		process_gameover();
	}

	if(collision_type == PLAYER_ENEMY_BULLET_COLLISION)
	{
		std::cout << "PLAYER_ENEMY_BULLET_COLLISION" << std::endl;
		process_gameover();
	}

	if(collision_type == PLAYER_HELIPORT_COLLISION)
	{
		std::cout << "PLAYER_HELIPORT_COLLISION" << std::endl;
		process_next_level(1);
	}
}

void Game::process_gameover()
{
	std::cout << "Game Over\n";
	running = false;
}

void Game::process_next_level(int level_number)
{
	std::cout << "Next Level\n";
	running = false;
}

void Game::destroy()
{
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}