#include "environment.hpp"

namespace StardustXRServer {

Item::TypeInfo EnvironmentItem::itemTypeInfo = {
	"environment",
	{
		"applySurfaceMaterial",
		"getData"
	}
};

EnvironmentItem::EnvironmentItem(Client *client, std::string path, pose_t pose) :
	Item(client, itemTypeInfo, pose),
	path(path) {}
EnvironmentItem::~EnvironmentItem() {}

void EnvironmentItem::serializeData(flexbuffers::Builder &fbb) {
	fbb.Vector([&]() {
		fbb.String(path);
	});
}

}
