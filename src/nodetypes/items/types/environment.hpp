#pragma once

#include <string>
#include <vector>

#include "../item.hpp"

namespace StardustXRServer {

class EnvironmentItem : public Item {
public:
	EnvironmentItem(Client *client, std::string path, sk::pose_t pose);
	~EnvironmentItem();

	void serializeData(flexbuffers::Builder &fbb);

	static TypeInfo itemTypeInfo;
protected:
	std::string path; //absolute plz
};

}
