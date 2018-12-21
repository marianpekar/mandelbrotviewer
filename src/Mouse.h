#pragma once

#include "SDL.h"

namespace marianpekar {
	class Mouse
	{
		Uint32 m_Coords{ 0 };
	public:
		void update();
		int m_x{ 0 };
		int m_y{ 0 };
		Mouse();
		~Mouse();
	};

}

