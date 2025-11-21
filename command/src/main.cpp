#include <ostream>
#include <iostream>
#include <string>
#include <filesystem>

#include "toml.hpp"
#include "json.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;

#define ORANGE1 "\e[38;5;208m"
#define ORANGE2 "\e[38;5;214m"
#define ORANGE3 "\e[38;5;220m"
#define RESET "\e[0m"

#define SERR cerr << ORANGE1 << "ERROR: " <<
#define EERR << RESET << '\n';

#define VERSION "0.0.0"
#define debug_mode ON

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
		SERR "Cannot find config folder path (APPDATA not set)" EERR
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
		SERR "Cannot find home folder" EERR;
		return "";
	}

#else

	#error "Unsupported platform"

#endif

#define REPO_DOMAIN "raw.githubusercontent.com"
#define REPO_PATH "/pg255/pix128/refs/heads/main"

string config_path;


typedef char download_response;
// everything successful: 0
// file/folder not found: 1
// error downloading file/folder: 2
// error saving file/folder: 3

struct downloaded_file {
	std::string file;
	download_response found = 2;
};

downloaded_file download_and_read_file(std::string from) {
	httplib::SSLClient cli(REPO_DOMAIN);

	cli.set_follow_location(true);
	cli.enable_server_certificate_verification(true);

	auto res = cli.Get(REPO_PATH + from);

	downloaded_file file = {};

	if (res && res->status == 200) {
		file.file = res->body;
		file.found = 0;
	} else if (res && res->status == 404) {
		file.found = 1;
	} else if (res) {
		SERR "Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with status: " << res->status EERR;
		file.found = 2;
	} else {
		SERR "Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with error: " << httplib::to_string(res.error()) EERR;
		file.found = 2;
	}
	return file;
}

download_response download_file(std::string from, std::string to) {
	httplib::SSLClient cli(REPO_DOMAIN);

	cli.set_follow_location(true);
	cli.enable_server_certificate_verification(true);

	auto res = cli.Get(REPO_PATH + from);
	if (res && res->status == 200) {
		fs::create_directories(
			fs::path(config_path + to).parent_path()
		);

		std::ofstream f(config_path + to, std::ios::binary);
		
		if (!f.is_open()) {
			SERR "Error: cannot open file for writing: " << config_path + to EERR;
			return 3;
		}
		
		if (f.fail() || f.bad()) {
			SERR "Error: writing to file failed: " << config_path + to EERR;
			return 3;
		}

		f.close();
		f << res->body;
		return 0;
	} else if (res && res->status == 404) {
		return 1;
	} else if (res) {
		SERR "Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with status: " << res->status EERR;
		return 2;
	} else {
		SERR "Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with error: " << httplib::to_string(res.error()) EERR;
		return 2;
	}
}

download_response download_folder(std::string from, std::string to) {

	return 0;
}

int main(int argc, char* argv[]) {
	config_path = getConfigFolderPath() + "/pix128";
	if (!fs::exists(config_path)) {
		try {
			fs::create_directory(config_path);
			fs::create_directory(config_path + "/templates");
			fs::create_directory(config_path + "/engines");
			fs::create_directory(config_path + "/libraries");

			cout << ORANGE2 << "Making config folder..." << RESET << endl;
			cout << ORANGE3 << "Welcome to Pix128!" << RESET << endl;
		} catch (const std::filesystem::filesystem_error& e) {
			SERR "error while creating config folder: " << e.what() EERR;
		}
	}

	if (argc == 1) {
		cout << "Pix128 " << VERSION << "\nRun --help for help" << endl;
		return 0;
	}
	if (std::string(argv[1]) == "--help") {

	}
	if (std::string(argv[1]) == "--version") {
		cout << "Pix128 " << VERSION << endl;
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
			downloaded_file test = download_and_read_file("/libraries/template/library.toml");
			if (test.found == 0) {
				auto tomf = toml::parse(test.file);
				log(tomf["id"].value_or(""));
				log(tomf["description"].value_or(""));
				log(tomf["version"].value_or(0));
			} else if (test.found == 1) {
				cout << "Library \"" << "test" << "\" not found" << endl;
			}
		}
		if (std::string(argv[1]) == "test2") {
			download_response response = download_file("/libraries/template/library.toml", "/libraries/library.toml");
			if (response == 0) {
				cout << "done" << endl;
			} else if (response == 1) {
				cout << "Library \"" << "test" << "\" not found" << endl;
			}
		}
	#endif

	return 0;
}
