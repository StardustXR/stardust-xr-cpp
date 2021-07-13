#pragma once

#include <stardustxr/messenger.hpp>
#include <stardustxr/server/messenger.hpp>

namespace StardustXRServer {

class Client;

class Messenger : public StardustXR::ServerMessenger {
public:
	explicit Messenger(Client *client, int readFD, int writeFD);
	virtual ~Messenger();

	void onPipeBreak();
private:
	Client *client;
};

}