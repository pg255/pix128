#include <ostream>
#include <iostream>
#include <string>
#include <filesystem>
#include <bits/stdc++.h>

#include "toml.hpp"
#include "json.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

namespace fs = std::filesystem;
using std::cout, std::cerr, std::cin, std::endl, std::string;

#define ORANGE1 "\e[38;5;208m"
#define ORANGE2 "\e[38;5;214m"
#define ORANGE3 "\e[38;5;220m"
#define DEBUGCOLOR "\e[0;36m"
#define RESET "\e[0m"

#define SERR cerr << ORANGE1 << "ERROR: " <<
#define EERR << RESET << '\n';

#define VERSION "0.0.0"


#define bl(t1) cout << DEBUGCOLOR << "#" << __LINE__ << " " << t1; cin.get(); cout << RESET << endl;
#define bl2(t1, t2) cout << DEBUGCOLOR << "#" << __LINE__ << " " << t1 << " : " << t2; cin.get(); cout << RESET << endl;
#define bl3(t1, t2, t3) cout << DEBUGCOLOR << "#" << __LINE__ << " " << t1 << " : " << t2 << " : " << t3; cin.get(); cout << RESET << endl;
#define jlog(t1) cout << DEBUGCOLOR << "#" << __LINE__ << " " << t1 << RESET << endl;
#define pb pl("bp")


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
	download_response response = 2;
};

downloaded_file download_and_read_file(std::string from) {
	httplib::SSLClient cli(REPO_DOMAIN);

	cli.set_follow_location(true);
	cli.enable_server_certificate_verification(true);

	auto res = cli.Get(REPO_PATH + from);

	downloaded_file file = {};

	if (res && res->status == 200) {
		file.file = res->body;
		file.response = 0;
	} else if (res && res->status == 404) {
		file.response = 1;
	} else if (res) {
		SERR "Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with status: " << res->status EERR;
		file.response = 2;
	} else {
		SERR "Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with error: " << httplib::to_string(res.error()) EERR;
		file.response = 2;
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
		SERR "File " << from << " not found" EERR
	} else if (res) {
		SERR "Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with status: " << res->status EERR;
		return 2;
	} else {
		SERR "Requesting '" << REPO_DOMAIN << REPO_PATH << from << "' failed with error: " << httplib::to_string(res.error()) EERR;
		return 2;
	}
	
	return 0;
}

download_response download_folder(std::string path) {
	downloaded_file file_list = download_and_read_file(path + "/.pix128files");
	if (file_list.response == 0) {
		std::stringstream ss(file_list.file);
		string file_name;
		while (std::getline(ss, file_name, '\n')) {
			download_response file_response = download_file(path + "/" + file_name, path + "/" + file_name);
			if (file_response == 0) {
				continue;
			} else {
				return file_response;
			}
		}
	} else {
		return file_list.response;
	}

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
			if (test.response == 0) {
				auto tomf = toml::parse(test.file);
				jlog(tomf["id"].value_or(""));
				jlog(tomf["description"].value_or(""));
				jlog(tomf["version"].value_or(0));
			} else if (test.response == 1) {
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
		if (std::string(argv[1]) == "test3") {
			download_folder("/templates/template/");
		}
	#endif

	return 0;
}