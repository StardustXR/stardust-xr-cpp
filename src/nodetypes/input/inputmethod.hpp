#ifndef STARDUSTXRSERVER_INPUTMETHOD_HPP
#define STARDUSTXRSERVER_INPUTMETHOD_HPP

#include "../node.hpp"
#include <stereokit.h>
#include <variant>

using namespace std;
using namespace sk;
namespace StardustXRServer {

class InputMethod : public Node {
public:
	InputMethod();
	virtual ~InputMethod();

	vector<uint8_t> modifyDatamap(uint sessionID, flexbuffers::Reference data, bool returnValue);

	virtual vector<uint8_t> serialize(float distance) = 0;
protected:
	typedef variant<bool, int32_t, float, vec2, vec3> DatamapVariant;
	map<string, DatamapVariant> datamap;

	DatamapVariant flexRefToVar(flexbuffers::Reference ref);
	void varToFlex(flexbuffers::Builder &fbb, DatamapVariant var);
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_INPUTMETHOD_HPP
