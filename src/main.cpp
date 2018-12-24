// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include "Screen.h"
#include "Mouse.h"
#include "FractalCreator.h"
#include "ConfigLoader.h"

using namespace marianpekar;

const int SLEEP_MILISECONDS = 200;

void renderer(std::shared_ptr<Screen> screen, std::shared_ptr<ConfigLoader> configLoader, bool& draw) {
	if (screen->init() == false) {
		std::cout << "Error initializing SDL" << std::endl;
	}

	while (true) {
		std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_MILISECONDS));

		if (draw) {
			screen->update();
			draw = false;
		}

		if (screen->processEvents() == false) {
			break;
		}
	}
}

void fractalWorker(std::shared_ptr<Screen> screen, std::shared_ptr<ConfigLoader> configLoader, std::shared_ptr<std::vector<Zoom>> zooms, bool& draw, bool& isFractalCalculated) {

	std::shared_ptr<Mandelbrot> mandelbrot(new Mandelbrot(configLoader->m_maxIterations));

	while (true) {
		std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_MILISECONDS));

		if (isFractalCalculated) {
			draw = false;
			std::unique_ptr<FractalCreator> fractalCreator(new FractalCreator(configLoader->m_fractalWidth, configLoader->m_fractalHeight, mandelbrot));

			for (auto range : configLoader->m_colorRanges) {
				fractalCreator->addRange(range.first, range.second);
			}

			if (zooms->size() > 0) {
				for (auto zoom : *zooms) {
					fractalCreator->addZoom(zoom);
				}
			}

			fractalCreator->draw(screen);
			isFractalCalculated = false;
			draw = true;
		}
	}
}

void events(std::shared_ptr<ConfigLoader> configLoader, std::shared_ptr<std::vector<Zoom>> zooms, bool& isFractalCalculated) {
	Mouse mouse;
	SDL_Event e;

	while (true) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_MOUSEBUTTONDOWN && !isFractalCalculated) {
				SDL_MouseButtonEvent& b = e.button;
				if (b.button == SDL_BUTTON_LEFT) {
					mouse.update();
					zooms->push_back(Zoom(mouse.m_x, mouse.m_y, configLoader->m_zoomScale));
					std::cout << "-> Zoom to " << mouse.m_x << "," << mouse.m_y << std::endl;
					isFractalCalculated = true;
				}
				else if (b.button == SDL_BUTTON_RIGHT) {
					if (zooms->size() >= 1) {
						zooms->pop_back();
						std::cout << "<- Unzoom" << std::endl;
						isFractalCalculated = true;
					}
				}
				else if (b.button == SDL_BUTTON_MIDDLE) {
					std::cout << "Screen size: " << configLoader->m_screenWidth << "x" << configLoader->m_fractalHeight << std::endl;
					std::cout << "Zoom scale: " << configLoader->m_zoomScale << std::endl;
					std::cout << "Fractal size: " << configLoader->m_fractalWidth << "x" << configLoader->m_fractalHeight << std::endl;
					std::cout << "Max iterations: " << configLoader->m_maxIterations << std::endl;
					std::cout << "Color ranges (RGB)" << std::endl;
					for (auto range : configLoader->m_colorRanges) {
						std::cout << " " << range.first << " : " << range.second.r << "," << range.second.g << "," << range.second.b << std::endl;
					}
				}
			} 
		}
	}
}

int main(int argc, char *args[])
{
	std::cout << "*------------------------------------*" << std::endl;
	std::cout << "|                                    |" << std::endl;
	std::cout << "|    Welcome to Mandelbrot Viewer    |" << std::endl;
	std::cout << "|             z = z2 + c             |" << std::endl;
	std::cout << "|                                    |" << std::endl;
	std::cout << "|   for zoom in point&click by LMB   |" << std::endl;
	std::cout << "|       for zoom out press RMB       |" << std::endl;
	std::cout << "|     print info by pressing MMB     |" << std::endl;
	std::cout << "|                                    |" << std::endl;
	std::cout << "*------------------------------------*" << std::endl;

	std::shared_ptr<ConfigLoader> configLoader(new ConfigLoader("config.json"));
	std::shared_ptr<Screen> screen(new Screen(configLoader->m_screenWidth, configLoader->m_screenHeight));
	std::shared_ptr<std::vector<Zoom>> zooms(new std::vector<Zoom>);
	bool isFractalCalculated = true;
	bool draw = false;

	std::thread rendererThread(renderer, screen, configLoader, std::ref(draw));
	std::thread fractalWorkerThread(fractalWorker, screen, configLoader, zooms, std::ref(draw), std::ref(isFractalCalculated));
	std::thread eventsThread(events, configLoader, zooms, std::ref(isFractalCalculated));

	rendererThread.join();
	fractalWorkerThread.join();
	eventsThread.join();
	return 0;
}
