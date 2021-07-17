#pragma once

#include <stardustxr/messenger.hpp>

namespace StardustXRServer {

class Client;

class Messenger : public StardustXR::Messenger {
public:
	explicit Messenger(Client *client, int readFD, int writeFD);
	virtual ~Messenger();

	void onPipeBreak();
private:
	Client *client;
};

}