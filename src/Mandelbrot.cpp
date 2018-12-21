#include "Mandelbrot.h"

namespace marianpekar {
	Mandelbrot::Mandelbrot(int maxIterations) :
		m_maxIterarions(maxIterations)
	{}

	Mandelbrot::~Mandelbrot() {}

	int Mandelbrot::getInerations(double x, double y) {
		std::complex<double> z = 0;
		std::complex<double> c(x, y);

		int iterations = 0;

		while (iterations < m_maxIterarions) {
			z = z * z + c;

			if (abs(z) > 2) {
				break;
			}

			iterations++;
		}

		return iterations;
	}
}

