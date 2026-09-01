#include <iostream>
#include <SDL3/SDL.h>

#include "settings.h"
#include "../radix/lib/map.h"
#include "../radix/lib/game.h"
#include "../radix/lib/entity.h"
#include "../radix/lib/component.h"
#include "../radix/lib/constants.h"
#include "../radix/lib/asset_manager.h"
#include "../radix/lib/entity_manager.h"
#include "../radix/lib/sprite_component.h"
#include "../radix/lib/collider_component.h"
#include "../radix/lib/keyboard_component.h"
#include "../radix/lib/transform_component.h"
#include "../radix/lib/text_label_component.h"
#include "../radix/lib/projectile_emitter_component.h"

namespace radix
{
	Map* map;
	SDL_Event Game::event;
	EntityManager entity_manager;
	SDL_Renderer* Game::renderer;
	SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	AssetManager* Game::asset_manager = new AssetManager(&entity_manager);

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

	bool Game::is_running() const
	{
		return this->running;
	}

	Entity& player(entity_manager.add_entity("player", PLAYER));

	void Game::load_level(int level_number)
	{
		asset_manager->add_font("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 24);
		asset_manager->add_texture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
		asset_manager->add_texture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
		asset_manager->add_texture("radar-image", std::string("./assets/images/radar.png").c_str());
		asset_manager->add_texture("heliport-image", std::string("./assets/images/heliport.png").c_str());
		asset_manager->add_texture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
		asset_manager->add_texture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());

		map = new Map("jungle-tiletexture", 2, 32);
		map->load_map("./assets/tilemaps/jungle.map", 25, 20);

		Entity& label_level_name(entity_manager.add_entity("LabelLevelName", UI));
		label_level_name.add_component<TextLabelComponent>(10, 10, "Level: 1", "charriot-font", WHITE);

		player.add_component<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
		player.add_component<SpriteComponent>("chopper-image", 2, 90, true, false);
		player.add_component<KeyboardComponent>("up", "down", "left", "right", "space");
		player.add_component<ColliderComponent>("PLAYER", 240, 106, 32, 32);

		Entity& tank(entity_manager.add_entity("tank", ENEMY));
		tank.add_component<TransformComponent>(250, 495, 5, 0, 32, 32, 1);
		tank.add_component<SpriteComponent>("tank-image");
		tank.add_component<ColliderComponent>("ENEMY", 150, 495, 32, 32);

		TransformComponent* tank_transform = tank.get_component<TransformComponent>();
		Entity& projectile(entity_manager.add_entity("projectile", PROJECTILE));
		projectile.add_component<TransformComponent>(tank_transform->position.x+16, tank_transform->position.y+16, 0, 0, 4, 4, 1);
		projectile.add_component<SpriteComponent>("projectile-image");
		projectile.add_component<ColliderComponent>("PROJECTILE", tank_transform->position.x+16, tank_transform->position.y+16, 4, 4);
		projectile.add_component<ProjectileEmitterComponent>(50, 0, 200, true);

		Entity& helipad(entity_manager.add_entity("helipad", OBSTACLE));
		helipad.add_component<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
		helipad.add_component<SpriteComponent>("heliport-image");
		helipad.add_component<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);

		Entity& radar(entity_manager.add_entity("radar", UI));
		radar.add_component<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
		radar.add_component<SpriteComponent>("radar-image", 8, 150, false, true);

		entity_manager.list_entities();
	}

	void Game::input()
	{
		SDL_PollEvent(&event);

		switch(event.type)
		{
			case SDL_EVENT_QUIT: { this->running = false; break; }
			case SDL_EVENT_KEY_DOWN: { if(event.key.key == SDLK_ESCAPE) this->running = false; break; }
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

		entity_manager.render();

		SDL_RenderPresent(this->renderer);
	}

	void Game::update_camera_movement()
	{
		TransformComponent* player_transform = player.get_component<TransformComponent>();

		camera.x = player_transform->position.x - static_cast<int>(WINDOW_WIDTH / 2);
		camera.y = player_transform->position.y - static_cast<int>(WINDOW_HEIGHT / 2);

		camera.x = (camera.x < 0) ? 0 : camera.x;
		camera.y = (camera.y < 0) ? 0 : camera.y;
		camera.x = (camera.x > camera.w) ? camera.w : camera.x;
		camera.y = (camera.y > camera.h) ? camera.h : camera.y;
	}

	void Game::check_collisions()
	{
		CollisionType collision_type = entity_manager.check_collisions();

		if(collision_type == PLAYER_ENEMY_COLLISION)
		{
			process_gameover();
		}

		if(collision_type == PLAYER_PROJECTILE_COLLISION)
		{
			process_gameover();
		}

		if(collision_type == PLAYER_LEVEL_COMPLETE_COLLISION)
		{
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
}