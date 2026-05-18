#include "pix128/meta/colors.hpp"

#include <string>
#include <iostream>


namespace pix128::log {

	void info(std::string msg) {
		std::cerr << COLOR_INFO << msg << COLOR_RESET << std::endl;
	}
	void warn(std::string msg) {
		std::cerr << COLOR_WARNING << msg << COLOR_RESET << std::endl;
	}
	void error(std::string msg) {
		std::cerr << COLOR_ERROR << msg << COLOR_RESET << std::endl;
	}

	std::string strong(std::string msg) {
		return COLOR_STRONG + msg + COLOR_RESET;		
	}

};
