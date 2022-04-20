#pragma once

#include <string>
#include <vector>

#include "../item.hpp"

class Surface;

namespace StardustXRServer {

class PanelItem : public Item {
public:
	PanelItem(Client *client, Surface *surface, sk::pose_t pose);
	~PanelItem();

	void serializeData(flexbuffers::Builder &fbb);
	std::vector<uint8_t> applySurfaceMaterial(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> setPointerActive(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setPointerPosition(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setPointerButtonPressed(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> scrollPointerAxis(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> touchDown(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> touchMove(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> touchUp(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> setKeyboardActive(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setKeymap(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setKeyState(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setKeyModStates(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setKeyRepeat(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> resize(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> close(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	static TypeInfo itemTypeInfo;
	Surface *surface;
};

}
