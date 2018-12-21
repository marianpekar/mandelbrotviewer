#pragma once

#include <complex>

namespace marianpekar {

	class Mandelbrot
	{
	public:
		int m_maxIterarions{ 1000 };

		Mandelbrot(int maxIterations);
		virtual ~Mandelbrot();
		int getInerations(double x, double y);
	};

}



