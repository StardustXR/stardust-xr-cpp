#pragma once

#include <string>
#include <vector>

#include "item.hpp"

namespace StardustXRServer {

class FileItem : public Item {
public:
	FileItem(Client *client, std::string path, std::string filetype, sk::pose_t pose);
	virtual ~FileItem();

	static FileItem FromFile(std::string path);

	const std::string filetype;
	static TypeInfo itemTypeInfo;
protected:
	std::string path; //absolute plz
};

}
