#include "spatial.hpp"

#include "../../globals.h"
#include "../../core/client.hpp"
#include "../../core/scenegraph.hpp"
#include "../../interfaces/spatial.hpp"
#include "../../nodetypes/core/alias.hpp"
#include "../../util/random.hpp"

#include <string>
#include <vector>

using namespace sk;

namespace StardustXRServer {

Registry<Spatial> Spatial::spatials;
std::map<uint32_t, sk::matrix> Spatial::anchors;

Spatial::Spatial(Client *client, matrix transform) : Node(client, false) {
	setTransformMatrix(transform);
}
Spatial::Spatial(Client *client, Spatial *spatialParent, matrix transform, bool translatable, bool rotatable, bool scalable, bool zoneable) :
		Spatial(client, transform) {
	this->spatialParent = spatialParent;
	this->translatable = translatable;
	this->rotatable = rotatable;
	this->scalable = scalable;

	STARDUSTXR_NODE_METHOD("createLaunchAnchor", &Spatial::createLaunchAnchor)

	STARDUSTXR_NODE_METHOD("getTransform", &Spatial::getTransform)
	STARDUSTXR_NODE_METHOD("setTransform", &Spatial::setTransform)

	STARDUSTXR_NODE_METHOD("setSpatialParent", &Spatial::setSpatialParentFlex)
	STARDUSTXR_NODE_METHOD("setSpatialParentInPlace", &Spatial::setSpatialParentInPlaceFlex)

	STARDUSTXR_NODE_METHOD("setZoneable", &Spatial::setZoneable)

	spatials.add(this);
	if(zoneable)
		Zone::zoneableSpatials.add(this);
}
Spatial::Spatial(Client *client, Spatial *spatialParent, pose_t transform, bool translatable, bool rotatable, bool scalable, bool zoneable) :
	Spatial(client, spatialParent, matrix_trs(transform.position, transform.orientation), translatable, rotatable, scalable, zoneable) {}
Spatial::~Spatial() {
	Zone::zoneableSpatials.remove(this);
	spatials.remove(this);
}

std::vector<uint8_t> Spatial::createLaunchAnchor(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!returnValue)
		return std::vector<uint8_t>();

	std::hash<uint32_t> idHasher;
	const uint32_t hashedID = idHasher(id);
	anchors[hashedID] = worldTransform();

	return FLEX_SINGLE(FLEX_UINT(hashedID));
}

std::vector<uint8_t> Spatial::getTransform(Client *callingClient, flexbuffers::Reference data, bool) {
	Spatial *space = callingClient->scenegraph.findNode<Spatial>(data.AsString().str());
	if(!space) {
		Alias *spaceAlias = callingClient->scenegraph.findNode<Alias>(data.AsString().str());
		space = spaceAlias ? spaceAlias->original.ptr<Spatial>() : nullptr;
	}
	vec3 pos, scl;
	quat rot;
	matrix_decompose(localToSpaceMatrix(space), pos, scl, rot);

	return StardustXR::FlexbufferFromArguments(
		FLEX_ARGS(
			if(translatable)
				FLEX_VEC3(pos)
			if(rotatable)
				FLEX_QUAT(rot)
			if(scalable)
				FLEX_VEC3(scl)
		)
	);
}
std::vector<uint8_t> Spatial::setTransform(Client *callingClient, flexbuffers::Reference data, bool) {
	flexbuffers::Vector flexVec = data.AsVector();
	std::string spaceString = flexVec[0].AsString().str();
	Spatial *space = nullptr;
	if(spaceString == "") {
		space = getSpatialParent();
	} else {
		Node *spaceNode = callingClient->scenegraph.findNode(spaceString);
		space = dynamic_cast<Spatial *>(spaceNode) ?: dynamic_cast<Alias *>(spaceNode)->original.ptr<Spatial>();
	}
	if(!space)
		return std::vector<uint8_t>();

	vec3 pos, scl;
	quat rot;
	matrix_decompose(transform, pos, scl, rot);

	if(translatable && flexVec[1].IsTypedVector() && flexVec[1].AsTypedVector().size() == 3) {
		flexbuffers::TypedVector posFlex = flexVec[1].AsTypedVector();
		pos.x = posFlex[0].AsFloat();
		pos.y = posFlex[1].AsFloat();
		pos.z = posFlex[2].AsFloat();
		pos = matrix_transform_pt(spaceToSpaceMatrix(space, getSpatialParent()), pos);
	}
	if(rotatable && flexVec[2].IsTypedVector() && flexVec[2].AsTypedVector().size() == 4) {
		flexbuffers::TypedVector rotFlex = flexVec[2].AsTypedVector();
		rot.x = rotFlex[0].AsFloat();
		rot.y = rotFlex[1].AsFloat();
		rot.z = rotFlex[2].AsFloat();
		rot.w = rotFlex[3].AsFloat();
		rot = matrix_transform_quat(spaceToSpaceMatrix(space, getSpatialParent()), rot);
	}
	if(scalable && flexVec[3].IsTypedVector() && flexVec[3].AsTypedVector().size() == 3) {
		flexbuffers::TypedVector sclFlex = flexVec[3].AsTypedVector();
		scl.x = sclFlex[0].AsFloat();
		scl.y = sclFlex[1].AsFloat();
		scl.z = sclFlex[2].AsFloat();
	}

	transform = matrix_trs(pos, rot, scl);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::setSpatialParentFlex(Client *callingClient, flexbuffers::Reference data, bool) {
	std::string spacePath = data.AsString().str();
	if(spacePath == "") {
		spatialParent = nullptr;
	} else {
		Spatial *potentialParent = callingClient->scenegraph.findNode<Spatial>(spacePath);
		if(!potentialParent) {
			Alias *potentialParentAlias = callingClient->scenegraph.findNode<Alias>(spacePath);
			if(potentialParentAlias && potentialParentAlias->original)
				potentialParent = potentialParentAlias->original.ptr<Spatial>();
		}
		setSpatialParent(potentialParent);
	}
	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setSpatialParentInPlaceFlex(Client *callingClient, flexbuffers::Reference data, bool) {
	std::string spacePath = data.AsString().str();
	if(spacePath == "") {
		spatialParent = nullptr;
	} else {
		Spatial *potentialParent = callingClient->scenegraph.findNode<Spatial>(spacePath);
		if(!potentialParent) {
			Alias *potentialParentAlias = callingClient->scenegraph.findNode<Alias>(spacePath);
			if(potentialParentAlias)
				potentialParent = potentialParentAlias->original.ptr<Spatial>();
		}
		setSpatialParentInPlace(potentialParent);
	}
	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::setZoneable(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(data.AsBool()) {
		Zone::zoneableSpatials.add(this);
	} else {
		Zone::zoneableSpatials.remove(this);
		if(this->zone)
			this->zone->release(std::to_string(id));
	}
	return std::vector<uint8_t>();
}

matrix Spatial::spaceToSpaceMatrix(Spatial *from, Spatial *to) {
	// TODO: Optimize this to check if from and to share a common ancestor
	// and calculate the transform matrix between the two.

	if(from == to)
		return matrix_identity;

	matrix spaceToWorldMatrix = matrix_identity;
	if(from)
		spaceToWorldMatrix = from->worldTransform();

	matrix worldToSpaceMatrix = matrix_identity;
	if(to)
		worldToSpaceMatrix = matrix_invert(to->worldTransform());

	return spaceToWorldMatrix * worldToSpaceMatrix;
}

matrix Spatial::localToSpaceMatrix(Spatial *space) {
	// TODO: Optimize this to check if space and this SpatialNode share a common ancestor
	// and calculate the transform matrix between the two.

	matrix localToWorldMatrix = worldTransform();

	if(space == nullptr)
		return localToWorldMatrix;

	matrix worldToSpaceMatrix = matrix_invert(space->worldTransform());

	return localToWorldMatrix * worldToSpaceMatrix;
}

matrix Spatial::spaceToLocalMatrix(Spatial *space) {
	// TODO: Optimize this to check if space and this SpatialNode share a common ancestor
	// and calculate the transform matrix between the two.

	matrix worldToLocalMatrix = matrix_invert(worldTransform());

	if(space == nullptr)
		return worldToLocalMatrix;

	matrix spaceToWorldMatrix = space->worldTransform();

	return spaceToWorldMatrix * worldToLocalMatrix;
}

Spatial *Spatial::getSpatialParent() {
	return spatialParent ? spatialParent.ptr() : &client->scenegraph.root;
}

bool Spatial::isAncestorOf(Spatial *spatial) {
	Spatial *currentParent = spatial;
	while(currentParent->id != currentParent->client->scenegraph.root.id) {
		if(currentParent->getSpatialParent()->id == this->id)
			return true;
		currentParent = currentParent->getSpatialParent();
	}
	return false;
}
bool Spatial::setSpatialParent(Spatial *spatial) {
	if(spatial == spatialParent)
		return false;

	if(!this->isAncestorOf(spatial))
		spatialParent = spatial;
	return true;
}
bool Spatial::setSpatialParentInPlace(Spatial *spatial) {
	if(spatial == spatialParent)
		return false;

	if(!this->isAncestorOf(spatial)) {
		setTransformMatrix(localToSpaceMatrix(spatial));
		spatialParent = spatial;
	}
	return true;
}
void Spatial::setTransformMatrix(matrix mat) {
	transform = mat;
}

matrix Spatial::localTransform() {
	return transform;
}

matrix Spatial::worldTransform() {
	if(spatialParent)
		return localTransform() * spatialParent->worldTransform();
	else
		return localTransform();
}

} // namespace StardustXRServer
