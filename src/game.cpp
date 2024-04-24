#include <iostream>

#include "../radix/headers/map.h"
#include "../radix/headers/game.h"
#include "../radix/headers/entity.h"
#include "../radix/headers/component.h"
#include "../radix/headers/constants.h"
#include "../radix/headers/asset_manager.h"
#include "../radix/headers/entity_manager.h"
#include "../radix/headers/sprite_component.h"
#include "../radix/headers/collider_component.h"
#include "../radix/headers/keyboard_component.h"
#include "../radix/headers/transform_component.h"
#include "../radix/headers/text_label_component.h"
#include "../radix/headers/projectile_emitter_component.h"

namespace Radix
{
	Map* map;
	SDL_Event Game::event;
	EntityManager entity_manager;
	SDL_Renderer* Game::renderer;
	SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	AssetManager* Game::asset_manager = new AssetManager(&entity_manager);

	Game::Game()
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::cerr << "Error Initializing SDL\n";
			return;
		}

		if(TTF_Init() != 0)
		{
			std::cerr << "Error Initializing SDL TTF\n";
			return;
		}

		this->window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);

		if(this->window == nullptr)
		{
			std::cerr << "Error Creating SDL window\n";
			return;
		}

		this->renderer = SDL_CreateRenderer(this->window, -1, 0);

		if(this->renderer == nullptr)
		{
			std::cerr << "Error Creating SDL Renderer";
			return;
		}

		LoadLevel(0);

		this->running = true;
		return;
	}

	Game::~Game()
	{}

	bool Game::Running() const
	{
		return this->running;
	}

	Entity& player(entity_manager.AddEntity("player", PLAYER));

	void Game::LoadLevel(int level_number)
	{
		asset_manager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 24);
		asset_manager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
		asset_manager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
		asset_manager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
		asset_manager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
		asset_manager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
		asset_manager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());

		map = new Map("jungle-tiletexture", 2, 32);
		map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

		Entity& label_level_name(entity_manager.AddEntity("LabelLevelName", UI));
		label_level_name.AddComponent<TextLabelComponent>(10, 10, "Level: 1", "charriot-font", WHITE);

		player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
		player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
		player.AddComponent<KeyboardComponent>("up", "down", "left", "right", "space");
		player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

		Entity& tank(entity_manager.AddEntity("tank", ENEMY));
		tank.AddComponent<TransformComponent>(250, 495, 5, 0, 32, 32, 1);
		tank.AddComponent<SpriteComponent>("tank-image");
		tank.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

		TransformComponent* tank_transform = tank.GetComponent<TransformComponent>();
		Entity& projectile(entity_manager.AddEntity("projectile", PROJECTILE));
		projectile.AddComponent<TransformComponent>(tank_transform->position.x+16, tank_transform->position.y+16, 0, 0, 4, 4, 1);
		projectile.AddComponent<SpriteComponent>("projectile-image");
		projectile.AddComponent<ColliderComponent>("PROJECTILE", tank_transform->position.x+16, tank_transform->position.y+16, 4, 4);
		projectile.AddComponent<ProjectileEmitterComponent>(50, 0, 200, true);

		Entity& helipad(entity_manager.AddEntity("helipad", OBSTACLE));
		helipad.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
		helipad.AddComponent<SpriteComponent>("heliport-image");
		helipad.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);

		Entity& radar(entity_manager.AddEntity("radar", UI));
		radar.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
		radar.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

		entity_manager.ListEntities();
	}

	void Game::Input()
	{
		SDL_PollEvent(&event);

		switch(event.type)
		{
			case SDL_QUIT: { this->running = false; break; }
			case SDL_KEYDOWN: { if(event.key.keysym.sym == SDLK_ESCAPE) this->running = false; break; }
			default: break;
		}
	}

	void Game::Update()
	{
		float delta_time = (SDL_GetTicks() - ticks_last_frame) / 1000.00f;

		delta_time = (delta_time > 0.05f) ? 0.05f : delta_time;

		this->ticks_last_frame = SDL_GetTicks();

		entity_manager.Update(delta_time);

		UpdateCameraMovement();
		CheckCollisions();
	}

	void Game::Render()
	{
		SDL_SetRenderDrawColor(this->renderer, 21, 21, 21, 255);
		SDL_RenderClear(this->renderer);

		if(entity_manager.IsEmpty() == true)
		{
			return;
		}

		entity_manager.Render();

		SDL_RenderPresent(this->renderer);
	}

	void Game::UpdateCameraMovement()
	{
		TransformComponent* player_transform = player.GetComponent<TransformComponent>();

		camera.x = player_transform->position.x - static_cast<int>(WINDOW_WIDTH / 2);
		camera.y = player_transform->position.y - static_cast<int>(WINDOW_HEIGHT / 2);

		camera.x = (camera.x < 0) ? 0 : camera.x;
		camera.y = (camera.y < 0) ? 0 : camera.y;
		camera.x = (camera.x > camera.w) ? camera.w : camera.x;
		camera.y = (camera.y > camera.h) ? camera.h : camera.y;
	}

	void Game::CheckCollisions()
	{
		CollisionType collision_type = entity_manager.CheckCollisions();

		if(collision_type == PLAYER_ENEMY_COLLISION)
		{
			ProcessGameOver();
		}

		if(collision_type == PLAYER_PROJECTILE_COLLISION)
		{
			ProcessGameOver();
		}

		if(collision_type == PLAYER_LEVEL_COMPLETE_COLLISION)
		{
			ProcessNextLevel(1);
		}
	}

	void Game::ProcessGameOver()
	{
		std::cout << "Game Over\n";
		running = false;
	}

	void Game::ProcessNextLevel(int level_number)
	{
		std::cout << "Next Level\n";
		running = false;
	}

	void Game::Destroy()
	{
		SDL_DestroyRenderer(this->renderer);
		SDL_DestroyWindow(this->window);
		SDL_Quit();
	}
}