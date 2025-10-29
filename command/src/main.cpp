#include <ostream>
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using std::cout;
using std::cin;
using std::endl;
using std::string;

#define VERSION "0.0.0"
#define debug_mode ON

#define ORANGE1 "\e[38;5;208m"
#define ORANGE2 "\e[38;5;214m"
#define ORANGE3 "\e[38;5;220m"
#define RESET "\e[0m"

#if debug_mode == ON

	#define DEB1 ORANGE1
	#define DEB2 ORANGE2
	#define DEB3 ORANGE3
	#define DEB_RESET RESET

	#define log(msg) cout << DEB1 << "#" << __LINE__ << " " << DEB3 << msg << DEB_RESET << endl;

	#define here cout << DEB1 << "#" << __LINE__ << DEB3 << " HERE" << DEB_RESET << endl;

	#define bp cout << DEB1 << "#" << __LINE__ << DEB2 << " [bp]" << DEB_RESET << cin.get() << endl;
	#define bp_its(value) cout << DEB1 << "#" << __LINE__ << " " << DEB3 << value << DEB2 << " [bp]" << DEB_RESET << cin.get() << endl;
	#define bp_is(name, value) cout << DEB1 << "#" << __LINE__ << DEB3 << " " << name << DEB1 << " is " << DEB3 << value << DEB2 << " [bp]" << DEB_RESET << cin.get() << endl;
#endif


#ifdef _WIN32

	#include <windows.h>

	std::string getConfigFolderPath() {
		const char* appdata = getenv("APPDATA");
		if (appdata) {
			return std::string(appdata);
		}
		// Fallback: try to get USERPROFILE
		const char* home = getenv("USERPROFILE");
		if (home) {
			return std::string(home) + "\\AppData\\Roaming";
		}
		cout << ORANGE1 << "ERROR: Cannot find config folder path (APPDATA not set)" << RESET << endl;
		return "";
	}

#elif __linux__

	#include <unistd.h>
	#include <pwd.h>

	std::string getConfigFolderPath() {
		const char* home = getenv("HOME");
		if (home) {
			return std::string(home) + "/.config";
		}
		cout << ORANGE1 << "ERROR: Cannot find home folder" << RESET << endl;
		return "";
	}

#else

	#error "Unsupported platform"

#endif

int main(int argc, char* argv[]) {
	string configPath = getConfigFolderPath() + "/pix128";
	if (!fs::exists(configPath)) {
		try {
			fs::create_directory(configPath);
			fs::create_directory(configPath + "/templates");
			fs::create_directory(configPath + "/engines");
			fs::create_directory(configPath + "/libraries");

			cout << ORANGE2 << "Making config folder..." << RESET << endl;
			cout << ORANGE3 << "Welcome to Pix128!" << RESET << endl;
		} catch (const std::filesystem::filesystem_error& e) {
			cout << ORANGE1 << "Error while creating config folder: " << e.what() << std::endl;
		}
    }

    if (argc == 1) {
		cout << "Pix128 " << VERSION << "\nRun --help for help" << endl;
		return 0;
	}
	if (std::string(argv[1]) == "--help") {
		
	}
	if (std::string(argv[1]) == "--version") {
		cout  << "Pix128 " << VERSION << endl;
	}
	if (std::string(argv[1]) == "engine") {
		
	}
	if (std::string(argv[1]) == "library") {
		
	}
	if (std::string(argv[1]) == "template") {
		
	}
	if (std::string(argv[1]) == "new") {
		
	}
	if (std::string(argv[1]) == "project") {
		
	}
    return 0;
}