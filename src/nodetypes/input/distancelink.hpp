#ifndef STARDUSTXRSERVER_DISTANCELINK_HPP
#define STARDUSTXRSERVER_DISTANCELINK_HPP

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

#endif // STARDUSTXRSERVER_DISTANCELINK_HPP
