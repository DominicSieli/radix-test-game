#ifndef MAP
#define MAP

#include <string>

namespace Radix
{
	class Map
	{
		private:
			int scale;
			int tile_size;
			std::string texture_id;

		public:
			Map(std::string, int, int);

			~Map();

			void LoadMap(std::string, int, int);

			void AddTile(int, int, int, int);
	};
}

#endif