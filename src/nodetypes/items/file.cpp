#include "file.hpp"

namespace StardustXRServer {

Item::TypeInfo FileItem::itemTypeInfo = {"file"};

FileItem::FileItem(Client *client, std::string path, std::string filetype, pose_t pose) :
	Item(client, itemTypeInfo, pose),
	filetype(filetype),
	path(path) {}
FileItem::~FileItem() {}

//FileItem FileItem::FromFile(std::string path) {
//	return FileItem(path); // gotta get the appropriate subclass based on file extension or type or whatever
//}

}
