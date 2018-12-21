#include "FractalCreator.h"

namespace marianpekar {

	void FractalCreator::draw(std::shared_ptr<Screen> screen) {
		calculateIteration();
		calculateTotalIterations();
		calculateRangeTotals();
		drawFractal(screen);
	}

	void FractalCreator::addRange(double rangeEnd, const RGB& rgb)
	{
		m_ranges.push_back(rangeEnd*m_mandlebrot->m_maxIterarions);
		m_colors.push_back(rgb);

		if (m_bGotFirstRange) {
			m_rangeTotals.push_back(0);
		}

		m_bGotFirstRange = true;
	}

	void FractalCreator::addZoom(const Zoom& zoom) {
		m_zoomList.add(zoom);
	}


	void FractalCreator::calculateRangeTotals()
	{
		int rangeIndex = 0;

		for (int i = 0; i < m_mandlebrot->m_maxIterarions; i++) {
			int pixels = m_histogram[i];

			if (i >= m_ranges[rangeIndex + 1]) {
				rangeIndex++;
			}

			m_rangeTotals[rangeIndex] += pixels;
		}
	}

	int FractalCreator::getRange(int iterations) const
	{
		int range = 0;

		for (int i = 1; i < m_ranges.size(); i++) {
			range = i;

			if (m_ranges[i] > iterations) {
				break;
			}
		}

		range--;

		return range;
	}

	FractalCreator::FractalCreator(int width, int height, std::shared_ptr<Mandelbrot> mandelbrot) :
		m_width(width),
		m_height(height),
		m_histogram(new int[mandelbrot->m_maxIterarions]{ 0 }),
		m_fractal(new int[width*height]{ 0 }),
		m_zoomList(width, height),
		m_mandlebrot(mandelbrot)
	{
		m_zoomList.add(Zoom(m_width / 2, m_height / 2, 4.0 / m_width));
	}

	FractalCreator::~FractalCreator()
	{
	}

	void FractalCreator::calculateIteration() {
		for (int y = 0; y < m_height; y++) {
			for (int x = 0; x < m_width; x++) {
				std::pair<double, double> coords = m_zoomList.doZoom(x, y);

				int iterations = m_mandlebrot->getInerations(coords.first, coords.second);
				m_fractal[y*m_width + x] = iterations;

				if (iterations != m_mandlebrot->m_maxIterarions) {
					m_histogram[iterations]++;
				}
			}
		}
	}

	void FractalCreator::calculateTotalIterations() {
		for (int i = 0; i < m_mandlebrot->m_maxIterarions; i++) {
			m_total += m_histogram[i];
		}
	}

	void FractalCreator::drawFractal(std::shared_ptr<Screen> screen) {
		RGB startColor(0, 0, 0);
		RGB endColor(255, 128, 255);
		RGB colorDiff = endColor - startColor;

		for (int y = 0; y < m_height; y++) {
			for (int x = 0; x < m_width; x++) {

				int iterations = m_fractal[y*m_width + x];

				int range = getRange(iterations);
				int rangeTotal = m_rangeTotals[range];
				int rangeStart = m_ranges[range];

				RGB& startColor = m_colors[range];
				RGB& endColor = m_colors[range + 1];
				RGB colorDiff = endColor - startColor;

				uint8_t red = 0;
				uint8_t green = 0;
				uint8_t blue = 0;

				if (iterations != m_mandlebrot->m_maxIterarions) {

					int totalPixels = 0;

					for (int i = rangeStart; i <= iterations; i++) {
						totalPixels += m_histogram[i];
					}

					red = startColor.r + colorDiff.r*(double)totalPixels / rangeTotal;
					green = startColor.g + colorDiff.g*(double)totalPixels / rangeTotal;
					blue = startColor.b + colorDiff.b*(double)totalPixels / rangeTotal;

				}

				screen->setPixel(x, y, red, green, blue);
			}
		}
	}

}
