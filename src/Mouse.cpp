#include "Mouse.h"

namespace marianpekar {
	Mouse::Mouse() {}

	Mouse::~Mouse() {}

	void Mouse::update() {
		m_Coords = SDL_GetMouseState(&m_x, &m_y);
	}
}

