#ifndef ANIMATION
#define ANIMATION

namespace Radix
{
	class Animation
	{
		public:
			unsigned int index;
			unsigned int speed;
			unsigned int frames;

			Animation();

			Animation(unsigned int, unsigned int, unsigned int);
	};
}

#endif