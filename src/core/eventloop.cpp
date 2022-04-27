#include "eventloop.hpp"
#include "../nodetypes/drawable/drawable.hpp"
#include "scenegraphpropagation.hpp"
#include <memory>
#include <mutex>
#include <vector>

namespace StardustXRServer {

EventLoop::EventLoop(bool &successful) :
epollFD(epoll_create1(0)),
socketHandler(this->successful){
	successful = this->successful;
	if(!successful) return;

	eventThread = std::thread(&EventLoop::eventLoop, this);
}
EventLoop::~EventLoop() {
	if(this->successful) {
		if(eventThread.joinable()) {
			socketHandler.close();
			eventThread.join();
		}
	}
	close(epollFD);
}

void EventLoop::addConnection(Connection *connection) {
	connections.emplace_back(connection);
	connection->startPoll();
}
void EventLoop::closeConnection(Connection *connection) {
	connections.erase(std::remove_if(connections.begin(), connections.end(), [connection](std::unique_ptr<Connection> &connectionPtr) {
						return connectionPtr.get() == connection;
					  }));
}

void EventLoop::callClientsDebug() {
	for(std::unique_ptr<Connection> &connection : connections) {
		Client *client = dynamic_cast<Client *>(connection.get());
		if(client)
			client->scenegraphPropagate("", ScenegraphDebugFunction);
	}
}
void EventLoop::eventLoop() {
	internalConnection = new Connection(this, socketHandler.fd(), EPOLLIN);
	addConnection(internalConnection);

	epoll_event events[20];
	while(1) {
		int eventCount = epoll_wait(epollFD, events, 20, 10000);
		if(eventCount == 0) continue;
		if(eventCount < 0) {
			socketHandler.close();
			return;
		}

		for(int i=0; i<eventCount; ++i) {
			Connection *connection = (Connection *) events[i].data.ptr;

			if(connection == internalConnection) {
				int fd = socketHandler.acceptNewClient();
				if(fd == -1) return;
				addConnection(new Client(this, fd));
				continue;
			}

			if(!connection->dispatch()) closeConnection(connection);
		}
	}
}

}
