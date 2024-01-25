#pragma once
#include <chrono>
#include <thread>
#include <memory>
#include <functional>
#include <ctime>
#include "event.h"

namespace Cinnamon {
	
	// Where T is std::chrono::seconds, milliseconds, etc.
	template<typename T = std::chrono::milliseconds>
	class Stopwatch {
	public:
		std::chrono::high_resolution_clock clock;
		std::chrono::high_resolution_clock::time_point start;

		inline void Start() {
			start = clock.now();
		}

		inline T Lap() {
			return std::chrono::duration_cast<T>(clock.now().time_since_epoch() - start.time_since_epoch());
		}

		inline void Reset() {
			clock.start = std::chrono::high_resolution_clock::time_point();
		}
	};
};
