#pragma once

#include <string>
#include <vector>

#include "item.hpp"

namespace StardustXRServer {

class FileItem : public Item {
public:
	FileItem(Client *client, std::string path, std::string filetype, sk::vec3 pos);
	virtual ~FileItem();

//	static FileItem FromFile(std::string path);

	const std::string filetype;
protected:
	std::string path; //absolute plz
	std::vector<uint8_t> data;
};

}
