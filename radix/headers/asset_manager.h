#ifndef ASSET_MANAGER
#define ASSET_MANAGER

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>

#include "game.h"
#include "font_manager.h"
#include "entity_manager.h"
#include "texture_manager.h"

namespace Radix
{
	class AssetManager
	{
		private:
			EntityManager* entity_manager;
			std::map<std::string, TTF_Font*> fonts;
			std::map<std::string, SDL_Texture*> textures;

		public:
			AssetManager(EntityManager*);

			~AssetManager();

			void Clear();

			SDL_Texture* GetTexture(std::string);

			void AddTexture(std::string, const char*);

			TTF_Font* GetFont(std::string);

			void AddFont(std::string, const char*, int);
	};
}

#endif