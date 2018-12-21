#pragma once

#include <iostream>
#include <memory>
#include <SDL.h>

namespace marianpekar {

	class Screen
	{
		int m_width{ 0 };
		int m_height{ 0 };
		SDL_Window* m_window{ nullptr };
		SDL_Renderer* m_renderer{ nullptr };
		SDL_Texture* m_texture{ nullptr };
		Uint32* m_buffer{ nullptr };
	public:
		Screen(int width, int height);
		virtual ~Screen();
		bool init();
		bool processEvents();
		void update();
		void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
		void close();
	};

}


