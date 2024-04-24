#ifndef GAME
#define GAME

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

namespace Radix
{
	class AssetManager;

	class Game
	{
		private:
			bool running = false;
			SDL_Window* window = nullptr;

		public:
			int ticks_last_frame = 0;
			static SDL_Event event;
			static SDL_Rect camera;
			static SDL_Renderer* renderer;
			static AssetManager* asset_manager;

			Game();

			~Game();

			bool Running() const;

			void LoadLevel(int);

			void Input();

			void Update();

			void Render();

			void UpdateCameraMovement();

			void CheckCollisions();

			void ProcessGameOver();

			void ProcessNextLevel(int);

			void Destroy();
	};
}

#endif