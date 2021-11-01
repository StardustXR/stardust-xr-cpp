#include "item.hpp"

namespace StardustXRServer {

Item::Item(std::string type) {
	this->type = type;
}

std::string Item::getType() {
	return type;
}

}
