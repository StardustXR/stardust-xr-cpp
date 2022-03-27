#pragma once

#include <string>
#include <stardustxr/common/flex.hpp>

namespace StardustXRServer {

class Client;

struct Callback {
	Client *client;
	std::string nodePath;
	std::string method;

	void signal(StardustXR::ArgsConstructor argsConstructor);
	void signal(std::vector<uint8_t> &flexbuffer);

	void executeMethod(StardustXR::ArgsConstructor argsConstructor, StardustXR::Callback callback);
	void executeMethod(std::vector<uint8_t> &flexbuffer, StardustXR::Callback callback);
};

}
