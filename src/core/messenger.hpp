#pragma once

#include <stardustxr/common/messenger.hpp>

namespace StardustXRServer {

class Client;

class Messenger : public StardustXR::Messenger {
public:
	explicit Messenger(Client *client, int fd);
	virtual ~Messenger();

	void onPipeBreak();
private:
	Client *client;
};

}
