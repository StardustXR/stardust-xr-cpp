#include "file.hpp"

namespace StardustXRServer {

FileItem::FileItem(Client *client, std::string path, std::string filetype, vec3 pos) :
	Item(client, "file", pos),
	filetype(filetype),
	path(path) {}
FileItem::~FileItem() {}

//FileItem FileItem::FromFile(std::string path) {
//	return FileItem(path); // gotta get the appropriate subclass based on file extension or type or whatever
//}

}
