#include "client.hpp"
#include "clientmanager.hpp"
#include "messenger.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

#include "../interfaces/environment.hpp"
#include "../interfaces/field.hpp"
#include "../interfaces/input.hpp"
#include "../interfaces/lifecycle.hpp"
#include "../interfaces/model.hpp"
#include "../interfaces/spatial.hpp"

namespace StardustXRServer {

Client::Client(int inFD, int outFD, ClientManager *clientManager) : scenegraph(this), messenger(this, inFD, outFD) {
	this->manager = clientManager;
	EnvironmentInterface *environment = new EnvironmentInterface(this);
	FieldInterface *field = new FieldInterface(this);
	InputInterface *input = new InputInterface(this);
	LifeCycleInterface *lifeCycle = new LifeCycleInterface(this);
	ModelInterface *model = new ModelInterface(this);
	SpatialFactory *spatial = new SpatialFactory(this);

	scenegraph.addNode("/environment", environment);
	scenegraph.addNode("/field", field);
	scenegraph.addNode("/input", input);
	scenegraph.addNode("/lifecycle", lifeCycle);
	scenegraph.addNode("/model", model);
	scenegraph.addNode("/spatial", spatial);
}
Client::~Client() {}

void Client::scenegraphPropagate(std::string name, PropagateFunction &function) {
	scenegraph.root.propagate(name, function);
}

void Client::startHandler() {
	messenger.startHandler();
}

}