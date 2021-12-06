#pragma once

#include <mutex>
#include <random>

namespace StardustXRServer {
namespace Random {

extern std::random_device rand_device;
extern std::mt19937 gen;
extern std::mutex randoMutex;

template <class T>
T generate() {
	std::lock_guard<std::mutex> lock(randoMutex);
	std::uniform_int_distribution<T> distrib;
	return distrib(gen);
}

}
}
