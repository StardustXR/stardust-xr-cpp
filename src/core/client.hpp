#pragma once

#include <atomic>

#include <stardustxr/common/messenger.hpp>
#include "connection.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

class Client : public Connection {
public:
	explicit Client(int epollFD, int fd);
	virtual ~Client();

	bool dispatch() override;

	void scenegraphPropagate(std::string name, PropagateFunction &function);

	const int fd;
	pid_t getPID();

	Scenegraph scenegraph;
	StardustXR::Messenger messenger;

};

}
