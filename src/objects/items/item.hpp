#pragma once

#include <string>
#include <vector>

namespace StardustXRServer {

class Item {
public:
	Item(std::string type);

	std::string getType();
private:
	std::string type;
};

}
