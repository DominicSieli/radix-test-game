#include <vector>
#include <iostream>
#include <SDL3/SDL.h>

#include "settings.h"

#include "player.h"
#include "colors.h"
#include "font_id.h"
#include "texture_id.h"
#include "asset_paths.h"
#include "collision_tags.h"

#include "../radix/src/game.h"
#include "../radix/src/entity.h"
#include "../radix/src/vector_2.h"
#include "../radix/src/tile_map.h"
#include "../radix/src/component.h"
#include "../radix/src/animation.h"
#include "../radix/src/collision.h"
#include "../radix/src/asset_manager.h"
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
Uint64 current_tick;
Uint64 previous_tick;
float Game::delta_time;
SDL_Event Game::input_event;
SDL_Renderer* Game::renderer;
EntityManager* Game::entity_manager;
AssetManager* Game::asset_manager;
RenderManager* Game::render_manager;
const unsigned int render_layer_max = 10;
CollisionManager* Game::collision_manager;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

using namespace Radix;

Game::Game()
{
	previous_tick = SDL_GetPerformanceCounter();

	Game::asset_manager = new AssetManager();
	Game::entity_manager = new EntityManager();
	Game::collision_manager = new CollisionManager();
	Game::render_manager = new RenderManager(render_layer_max);

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

void Game::load_level(unsigned int level_number)
{
	this->asset_manager->add_font(CHARRIOT_ID, CHARRIOT_TTF_PATH, 24);
	this->asset_manager->add_texture(TANK_TEXTURE_ID, TANK_PNG_PATH);
	this->asset_manager->add_texture(RADAR_TEXTURE_ID, RADAR_PNG_PATH);
	this->asset_manager->add_texture(HELIPORT_TEXTURE_ID, HELIPORT_PNG_PATH);
	this->asset_manager->add_texture(JUNGLE_MAP_TEXTURE_ID, JUNGLE_PNG_PATH);
	this->asset_manager->add_texture(ENEMY_BULLET_TEXTURE_ID, BULLET_PNG_PATH);

	player = create_player(Vector2<float>(240, 160));

	tile_map = new TileMap(JUNGLE_MAP_TEXTURE_ID, 2, 32);
	tile_map->load_map(JUNGLE_MAP_PATH, 25, 20, "tile", 0);

	Entity* level_name = this->entity_manager->add_entity("LabelLevelName", 9);
	level_name->add_component<TextComponent>(Vector2<float>(10, 10), "Level: 1", CHARRIOT_ID, WHITE);

	Entity* tank = this->entity_manager->add_entity("tank", 1);
	tank->add_component<TransformComponent>(Vector2<float>(250, 495), Vector2<float>(1, 1), 0);
	tank->add_component<StaticSpriteComponent>(TANK_TEXTURE_ID, Vector2<float>(32, 32), false);
	tank->add_component<ColliderComponent>(ENEMY_COLLIDER_TAG, Vector2<int>(32, 32));
	TransformComponent* tank_transform = tank->get_component<TransformComponent>();
	tank_transform->translate(Vector2<float>(10000, 0));

	Entity* bullet = this->entity_manager->add_entity("bullet", 1);
	bullet->add_component<TransformComponent>(Vector2<float>(tank_transform->position.x+16, tank_transform->position.y+16), Vector2<float>(1, 1), 0);
	bullet->add_component<StaticSpriteComponent>(ENEMY_BULLET_TEXTURE_ID, Vector2<float>(4, 4), false);
	bullet->add_component<ColliderComponent>(ENEMY_BULLET_COLLIDER_TAG, Vector2<int>(4, 4));
	TransformComponent* bullet_transform = bullet->get_component<TransformComponent>();
	bullet_transform->translate(Vector2<float>(2000, 0));

	Entity* heliport = this->entity_manager->add_entity("heliport", 1);
	heliport->add_component<TransformComponent>(Vector2<float>(470, 420), Vector2<float>(1, 1), 0);
	heliport->add_component<StaticSpriteComponent>(HELIPORT_TEXTURE_ID, Vector2<float>(32, 32), false);
	heliport->add_component<ColliderComponent>(HELIPORT_COLLIDER_TAG, Vector2<int>(32, 32));

	std::map<unsigned int, Animation> radar_animation;

	Animation rotate_radar = Animation(0, 8, 150);
	radar_animation.emplace(0, rotate_radar);

	Entity* radar = this->entity_manager->add_entity("radar", 9);
	radar->add_component<TransformComponent>(Vector2<float>(720, 15), Vector2<float>(1, 1), 0);
	radar->add_component<AnimatedSpriteComponent>(radar_animation, RADAR_TEXTURE_ID, 0, Vector2<int>(64, 64), true);
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
	current_tick = SDL_GetPerformanceCounter();

	delta_time = static_cast<float>(current_tick - previous_tick) / static_cast<float>(SDL_GetPerformanceFrequency());

	previous_tick = current_tick;

	this->entity_manager->update(delta_time);

	update_camera_movement();
	check_collisions();
}

void Game::render()
{
	SDL_SetRenderDrawColor(this->renderer, 21, 21, 21, 255);
	SDL_RenderClear(this->renderer);

	if(this->entity_manager->is_empty() == true)
	{
		return;
	}

	this->render_manager->render();

	SDL_RenderPresent(this->renderer);
}

void Game::update_camera_movement()
{
	TransformComponent* player_transform = player->get_component<TransformComponent>();

	camera.x = static_cast<int>(player_transform->position.x) - static_cast<int>(WINDOW_WIDTH / 2);
	camera.y = static_cast<int>(player_transform->position.y) - static_cast<int>(WINDOW_HEIGHT / 2);

	camera.x = (camera.x < 0) ? 0 : camera.x;
	camera.y = (camera.y < 0) ? 0 : camera.y;
	camera.x = (camera.x > camera.w) ? camera.w : camera.x;
	camera.y = (camera.y > camera.h) ? camera.h : camera.y;
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

	unsigned int collision_type = this->collision_manager->check_collisions(collisions, NO_COLLISION);

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

void Game::process_next_level(unsigned int level_number)
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