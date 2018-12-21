#include "Screen.h"

namespace marianpekar {
	
	Screen::Screen(int width, int height) : 
		m_width(width),
		m_height(height) 
	{}

	Screen::~Screen() {}

	bool Screen::init() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			return false;
		}

		m_window = SDL_CreateWindow("Mandelbrot Viewer",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			m_width,
			m_height,
			SDL_WINDOW_SHOWN);

		if (m_window == nullptr) {
			std::cout << SDL_GetError() << std::endl;
			SDL_Quit();
			return false;
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, m_width, m_height);

		if (m_renderer == nullptr) {
			std::cout << SDL_GetError() << std::endl;
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}

		if (m_texture == nullptr) {
			std::cout << SDL_GetError() << std::endl;
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}

		m_buffer = new Uint32[m_width * m_height];
		memset(m_buffer, 0, m_width * m_height * sizeof(Uint32));

		return true;
	}

	bool Screen::processEvents() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return false;
			}
		}

		return true;
	}

	void Screen::update() {
		SDL_UpdateTexture(m_texture, nullptr, m_buffer, m_width * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);
	}

	void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
		if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
			return;
		}

		Uint32 color = 0;

		color += red;
		color <<= 8;
		color += green;
		color <<= 8;
		color += blue;
		color <<= 8;
		color += 0xFF;

		m_buffer[(y * m_width) + x] = color;
	}

	void Screen::close() {
		delete[] m_buffer;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyTexture(m_texture);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

}
