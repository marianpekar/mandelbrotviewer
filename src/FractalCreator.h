#pragma once

#include <memory>
#include <vector>
#include <assert.h>

#include "Mandelbrot.h"
#include "ZoomList.h"
#include "RGB.h"
#include "Screen.h"

namespace marianpekar {

	class FractalCreator
	{
	private:
		int m_width;
		int m_height;
		int m_total{ 0 };
		std::unique_ptr<int[]> m_histogram;
		std::unique_ptr<int[]> m_fractal;
		std::shared_ptr<Mandelbrot> m_mandlebrot;
		ZoomList m_zoomList;

		std::vector<int> m_ranges;
		std::vector<RGB> m_colors;
		std::vector<int> m_rangeTotals;

		bool m_bGotFirstRange{ false };

		void drawFractal(std::shared_ptr<Screen> screen);
		void calculateTotalIterations();
		void calculateIteration();
		void calculateRangeTotals();
		int getRange(int iterations) const;
	public:
		FractalCreator(int width, int height, std::shared_ptr<Mandelbrot> mandlebrot);
		virtual ~FractalCreator();
		void draw(std::shared_ptr<Screen> screen);
		void addRange(double rangeEnd, const RGB& rgb);
		void addZoom(const Zoom& zoom);
	};

}


