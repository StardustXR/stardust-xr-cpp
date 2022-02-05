#include "alias.hpp"
#include "../../core/client.hpp"

namespace StardustXRServer {

SpatialAlias::SpatialAlias(Client *client, Spatial *original, std::vector<std::string> methods) :
	Alias(client, original, methods),
	spatialOriginal(original) {

	this->methods["move"] = original->methods["move"];
	this->methods["rotate"] = original->methods["rotate"];
	this->methods["scaleThis"] = original->methods["scaleThis"];

	this->methods["getTransform"] = original->methods["getTransform"];

	this->methods["setOrigin"] = original->methods["setOrigin"];
	this->methods["setOrientation"] = original->methods["setOrientation"];
	this->methods["setScale"] = original->methods["setScale"];
	this->methods["setPose"] = original->methods["setPose"];
	this->methods["setTransform"] = original->methods["setTransform"];
	this->methods["setZoneable"] = original->methods["setZoneable"];

	STARDUSTXR_NODE_METHOD("setSpatialParent", &SpatialAlias::setSpatialParentFlex)
	STARDUSTXR_NODE_METHOD("setSpatialParentInPlace", &SpatialAlias::setSpatialParentInPlaceFlex)
}

std::vector<uint8_t> SpatialAlias::setSpatialParentFlex(Client *callingClient, flexbuffers::Reference data, bool) {
	std::string spacePath = data.AsString().str();
	if(spacePath == "") {
		spatialOriginal->setSpatialParent(nullptr);
	} else {
		Spatial *potentialParent = client->scenegraph.findNode<Spatial>(spacePath);
		if(!potentialParent) {
			Alias *potentialParentAlias = client->scenegraph.findNode<Alias>(spacePath);
			if(potentialParentAlias)
				potentialParent = potentialParentAlias->original.ptr<Spatial>();
		}
		spatialOriginal->setSpatialParent(potentialParent);
	}
	return std::vector<uint8_t>();
}
std::vector<uint8_t> SpatialAlias::setSpatialParentInPlaceFlex(Client *callingClient, flexbuffers::Reference data, bool) {
	std::string spacePath = data.AsString().str();
	if(spacePath == "") {
		spatialOriginal->setSpatialParentInPlace(nullptr);
	} else {
		Spatial *potentialParent = client->scenegraph.findNode<Spatial>(spacePath);
		if(!potentialParent) {
			Alias *potentialParentAlias = client->scenegraph.findNode<Alias>(spacePath);
			if(potentialParentAlias)
				potentialParent = potentialParentAlias->original.ptr<Spatial>();
		}
		spatialOriginal->setSpatialParentInPlace(potentialParent);
	}
	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
