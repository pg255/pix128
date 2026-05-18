#include "pix128/log.hpp"
#include "pix128/meta/path.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <string>
#include <filesystem>
#include <fstream>


namespace pix128::network {

	struct path_response {
		char response;
		std::string path;
	};

	struct content_response {
		char response;
		std::string content;
	};

	pix128::network::content_response repository_read_file(std::string source_file_path) {

		httplib::SSLClient cli(PIX128_REPO_DOMAIN);

		cli.set_follow_location(true);
		cli.enable_server_certificate_verification(true);

		auto res = cli.Get(PIX128_REPO_PATH + source_file_path);

		content_response response;

		if (res && res->status == 200) {
			response.content = res->body;
			response.response = 0;
		} else if (res && res->status == 404) {
			response.response = 1;
		} else if (res) {
			pix128::log::error("Requesting \"" PIX128_REPO_DOMAIN PIX128_REPO_PATH + source_file_path + "\" failed with status: " + std::to_string(res->status));
			response.response = 1;
		} else {
			pix128::log::error("Requesting \"" PIX128_REPO_DOMAIN PIX128_REPO_PATH + source_file_path + "\" failed with error: " + httplib::to_string(res.error()));
			response.response = 1;
		}

		return response;
	}

	int download_file(std::string source_file_path, std::string destination_file_path) {
		
		httplib::SSLClient cli(PIX128_REPO_DOMAIN);

		cli.set_follow_location(true);
		cli.enable_server_certificate_verification(true);

		auto result = cli.Get(PIX128_REPO_PATH + source_file_path);
		if (result && result->status == 200) {
			std::filesystem::create_directories(
				std::filesystem::path(destination_file_path).parent_path()
			);

			std::ofstream destination_file(destination_file_path, std::ios::binary);

			if (!destination_file.is_open()) {
				pix128::log::error("Cannot open file for writing: " + destination_file_path);
				return 1;
			}

			if (destination_file.fail() || destination_file.bad()) {
				pix128::log::error("Writing to file failed: " + destination_file_path);
				return 1;
			}

			f << res->body;
			f.close();
			return SoResponse::success;
		} else if (res && res->status == 404) {
			return SoResponse::repoNotFound;
		} else if (res) {
			ERROR("Requesting '" << REPO_DOMAIN << REPO_PATH << source_file_path << "' failed with status: " << res->status);
			return SoResponse::repoError;
		} else {
			ERROR("Requesting '" << REPO_DOMAIN << REPO_PATH << source_file_path << "' failed with error: " << httplib::to_string(res.error()));
			return SoResponse::repoError;
		}

		return SoResponse::success;


	}

	void download_folder() {
		
	};

}
