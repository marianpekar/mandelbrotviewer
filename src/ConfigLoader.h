#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "json.hpp"
#include "RGB.h"

namespace marianpekar {
	class ConfigLoader
	{
	public:
		ConfigLoader(std::string filepath);
		virtual ~ConfigLoader();
		int m_screenWidth{ 800 };
		int m_screenHeight{ 600 };
		int m_fractalWidth{ 800 };
		int m_fractalHeight{ 600 };
		int m_maxIterations{ 1000 };
		std::vector<std::pair<double, RGB>> m_colorRanges;
		double m_zoomScale{ 0.2 };
	};
}

