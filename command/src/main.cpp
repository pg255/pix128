#include <iostream>
#include <string>
#include <filesystem>

//#include "debug255.hpp"
#include "toml.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

namespace fs = std::filesystem;

//// Debug255 ////

#define DEBUG255_COLOR "\e[0;36m"
#define DEBUG255_RESET "\e[0m"
#define DEBUG255_SYMBOL "#"
#define DEBUG255_SPLIT " : "

#define bl(t1) std::cout << DEBUG255_COLOR << DEBUG255_SYMBOL << __LINE__ << " " << t1; std::cin.get(); std::cout << DEBUG255_RESET << std::endl;
#define bl2(t1, t2) std::cout << DEBUG255_COLOR << DEBUG255_SYMBOL << __LINE__ << " " << t1 << DEBUG255_SPLIT << t2; std::cin.get(); std::cout << DEBUG255_RESET << std::endl;
#define bl3(t1, t2, t3) std::cout << DEBUG255_COLOR << DEBUG255_SYMBOL << __LINE__ << " " << t1 << DEBUG255_SPLIT << t2 << DEBUG255_SPLIT << t3; std::cin.get(); std::cout << DEBUG255_RESET << std::endl;
#define LOG(t1) std::cout << DEBUG255_COLOR << DEBUG255_SYMBOL << __LINE__ << " " << t1 << DEBUG255_RESET << std::endl;
#define bp bl("bp")

//// Project macros ////

#define VERSION "0.0.0"

#define ORANGE1 "\e[38;5;208m"
#define ORANGE2 "\e[38;5;214m"
#define ORANGE3 "\e[38;5;220m"
#define RESET "\e[0m"

#define ERROR(msg) std::cerr << ORANGE1 << "ERROR: " << msg << RESET << '\n'

#define REPO_DOMAIN "raw.githubusercontent.com"
#define REPO_PATH "/pg255/pix128/refs/heads/main"

//// Global variables ////

std::string configPath;

//// SO aka System Operations ///
 
 /* Functions:
  * downloadAndReadFile()
  * downloadFile()
  * downloadFolder()
  * 
  * createFile()
  * readFile()
  * editFile()
  * 
  * deleteFolder()
  * copyFolder()
  * 
  * listFolders()
  * getConfigFolder()
  */

enum class SoResponse {
	success,
	localNotFound,
	repoNotFound,
	repoError,
	localWritingError,
	otherError
};

struct SoContent {
	std::string content;
	SoResponse response;
};

struct SoPath {
	std::string path;
	SoResponse response;
};

SoContent downloadAndReadFile(std::string from) {
	httplib::SSLClient cli(REPO_DOMAIN);
	
	cli.set_follow_location(true);
	cli.enable_server_certificate_verification(true);

	auto res = cli.Get(REPO_PATH + from);

	SoContent file;

	if (res && res->status == 200) {
		file.content = res->body;
		file.response = SoResponse::success;
	} else if (res && res->status == 404) {
		file.response = SoResponse::repoNotFound;
	} else if (res) {
		ERROR("Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with status: " << res->status);
		file.response = SoResponse::repoError;
	} else {
		ERROR("Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with error: " << httplib::to_string(res.error()));
		file.response = SoResponse::repoError;
	}
	
	return file;
}

SoResponse downloadFile(std::string from, std::string to) {	
	
	httplib::SSLClient cli(REPO_DOMAIN);

	cli.set_follow_location(true);
	cli.enable_server_certificate_verification(true);

	auto res = cli.Get(REPO_PATH + from);
	if (res && res->status == 200) {
		fs::create_directories(
			fs::path(configPath + to).parent_path()
		);

		std::ofstream f(configPath + to, std::ios::binary);

		if (!f.is_open()) {
			ERROR("Error: cannot open file for writing: " << configPath + to);
			return SoResponse::localWritingError;
		}

		if (f.fail() || f.bad()) {
			ERROR("Error: writing to file failed: " << configPath + to);
			return SoResponse::localWritingError;
		}

		f << res->body;
		f.close();
		return SoResponse::success;
	} else if (res && res->status == 404) {
		return SoResponse::repoNotFound;
	} else if (res) {
		ERROR("Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with status: " << res->status);
		return SoResponse::repoError;
	} else {
		ERROR("Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with error: " << httplib::to_string(res.error()));
		return SoResponse::repoError;
	}
	
	return SoResponse::success;
}

SoResponse downloadFolder(std::string path) {
	SoContent fileList = downloadAndReadFile(path + "/.pix128files");
	
	if (fileList.response == SoResponse::success) {
		std::stringstream ss(fileList.content);
		std::string fileName;
		while (std::getline(ss, fileName, '\n')) {
			SoResponse fileResponse = downloadFile(path + "/" + fileName, path + "/" + fileName);
			if (fileResponse == SoResponse::success) {
				continue;
			} else if (fileResponse == SoResponse::repoNotFound) {
				ERROR("There is an error in " + path + "/.pix128files");
				return SoResponse::otherError;
			}
			return fileResponse;
		}
	} else {
		return fileList.response;
	}

	return SoResponse::success;
}

#ifdef _WIN32

	#include <windows.h>

	SoPath getConfigFolder() {
		SoPath path;
		
		const char* appdata = getenv("APPDATA");
		if (appdata) {
			path.path = std::string(appdata);
			path.response = SoResponse::success;
			return path;
		}
		
		// Fallback: try to get USERPROFILE
		const char* home = getenv("USERPROFILE");
		if (home) {
			path.path = std::string(home) + "\\AppData\\Roaming";
			path.response = SoResponse::success;
			return path;
		}
		
		path.response = SoResponse::localNotFound;
		ERROR("Cannot find config folder path (APPDATA not set)");
		return path;
	}

#elif __linux__

	#include <unistd.h>
	#include <pwd.h>

	SoPath getConfigFolder() {
		SoPath path;
		const char* home = getenv("HOME");
		if (home) {
			path.path = std::string(home) + "/.config";
			path.response = SoResponse::success;
			return path;
		}
		path.response = SoResponse::localNotFound;
		ERROR("Cannot find home folder");
		return path;
	}

#else

	#error "Unsupported platform"

#endif

//// main ////

int main(int argc, char* argv[]) {
	SoPath soConfigPath = getConfigFolder();
	if (soConfigPath.response != SoResponse::success) {
		ERROR("Can't continue without having config path");
		return 1;
	}
	
	configPath = soConfigPath.path + "/pix128";
	if (!fs::exists(configPath)) {
		try {
			fs::create_directory(configPath);
			fs::create_directory(configPath + "/templates");
			fs::create_directory(configPath + "/engines");
			fs::create_directory(configPath + "/libraries");

			std::cout << ORANGE2 << "Making config folder..." << RESET << std::endl;
			std::cout << ORANGE3 << "Welcome to Pix128!" << RESET << std::endl;
		} catch (const std::filesystem::filesystem_error& e) {
			ERROR("Error while creating config folder: " << e.what());
		}
	}

	if (argc == 1) {
		std::cout << "Pix128 " << VERSION << "\nRun --help for help" << std::endl;
		return 0;
	}
	if (std::string(argv[1]) == "--help") {

	}
	if (std::string(argv[1]) == "--version") {
		std::cout << "Pix128 " << VERSION << std::endl;
	}
	if (std::string(argv[1]) == "engine") {
		if (std::string(argv[2]) == "download") {
			
		}
		if (std::string(argv[2]) == "delete") {

		}
		if (std::string(argv[2]) == "update") {

		}
	}
	if (std::string(argv[1]) == "library") {

	}
	if (std::string(argv[1]) == "template") {

	}
	if (std::string(argv[1]) == "new") {

	}
	if (std::string(argv[1]) == "project") {

	}

	#if debug_mode == ON
		if (std::string(argv[1]) == "test1") {
			SoContent test = downloadAndReadFile("/libraries/template/library.toml");
			if (test.response == SoResponse::success) {
				auto tomf = toml::parse(test.content);
				LOG(tomf["id"].value_or(""));
				LOG(tomf["description"].value_or(""));
				LOG(tomf["version"].value_or(0));
			} else if (test.response == SoResponse::repoNotFound) {
				std::cout << "Library \"" << "test" << "\" not found" << std::endl;
			}
		}
		
		if (std::string(argv[1]) == "test2") {
			SoResponse response = downloadFile("/libraries/template/library.toml", "/libraries/library.toml");
			if (response == SoResponse::success) {
				LOG("YAY!");
			} else if (response == SoResponse::repoNotFound) {
				std::cout << "Library \"" << "test" << "\" not found" << std::endl;
			}
		}
		
		if (std::string(argv[1]) == "test3") {
			SoResponse folder = downloadFolder("/templates/template/");
			if (folder == SoResponse::success) {
				LOG("YAY!");
			}
		}
	#endif

	return 0;
}