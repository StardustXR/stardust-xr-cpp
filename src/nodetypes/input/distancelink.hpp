#pragma once

namespace StardustXRServer {

class InputMethod;
class InputHandler;

typedef struct{
	InputMethod *method;
	float distance;
	InputHandler *handler;
} DistanceLink;

inline bool operator<(DistanceLink &a, DistanceLink &b) {
	return a.distance < b.distance;
}

}
