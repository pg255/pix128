#pragma once

#include <string>

namespace pix128::log {

	void info(std::string msg);
	void warn(std::string msg);
	void error(std::string msg);

	std::string strong(std::string msg);

};
