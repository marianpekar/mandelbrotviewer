#include "ConfigLoader.h"

using json = nlohmann::json;

namespace marianpekar {

	ConfigLoader::ConfigLoader(std::string filepath)
	{
		std::ifstream configFile(filepath);
		std::stringstream buffer;
		buffer << configFile.rdbuf();
		json config = json::parse(buffer.str());

		m_screenWidth = config["Screen"]["Width"];
		m_screenHeight = config["Screen"]["Height"];
		m_zoomScale = config["Screen"]["ZoomScale"];
		m_fractalWidth = config["Fractal"]["Width"];
		m_fractalHeight = config["Fractal"]["Height"];
		m_maxIterations = config["Fractal"]["MaxIterations"];

		for (auto colorRange : config["Fractal"]["ColorRanges"]) {
			m_colorRanges.push_back(std::pair<double, RGB>(colorRange["Range"], RGB(colorRange["R"], colorRange["G"], colorRange["B"])));
		}

	}


	ConfigLoader::~ConfigLoader()
	{
	}

}
