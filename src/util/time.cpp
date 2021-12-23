#include "time.hpp"

#include <time.h>

namespace StardustXRServer {

uint32_t Time::timestampMS() {
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (now.tv_sec * 1000) + (now.tv_nsec / 1000000);
}

} // namespace StardustXRServer
