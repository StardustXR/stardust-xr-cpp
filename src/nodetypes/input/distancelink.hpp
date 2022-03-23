#pragma once

#include "../core/typednoderef.hpp"

namespace StardustXRServer {

class InputMethod;
class InputHandler;

typedef struct{
	TypedNodeRef<InputMethod> method;
	float distance;
	TypedNodeRef<InputHandler> handler;
} DistanceLink;

inline bool operator<(DistanceLink &a, DistanceLink &b) {
	return a.distance < b.distance;
}

}
