#pragma once
#include <chrono>
#include <thread>
#include <memory>
#include <functional>
#include <ctime>
#include "event.h"

namespace Cinnamon {
/*
	template<typename Duration = std::chrono::milliseconds, typename...EventArgs>
	class Timer {
	public:
		Event<EventArgs...> OnTick;
		Duration deltaTime = Duration(0);
		int interval;
		bool active = false;
		Duration timeLeft;
		Duration waitTime;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="interval">in Duration units</param>
		inline Timer(int interval = 1) {
			this->interval = interval;
			this->timeLeft = Duration(interval);
			this->waitTime = Duration(interval);
		}

		inline std::thread Start(int interval, EventArgs...args) {
			if (active)
				return;

			auto clock = std::chrono::steady_clock();
			this->timeLeft = Duration(interval);
			this->waitTime = timeLeft;
			this->active = true;

			return std::thread([&]() {
				active = true;
				while (active) {
					if (timeLeft < Duration(0)) {
						timeLeft += waitTime;
					}

					// Do OnTick and mark timepoints
					auto start = std::chrono::duration_cast<Duration>(clock.now().time_since_epoch());
					OnTick(std::forward<EventArgs>(args)...);
					auto end = std::chrono::duration_cast<Duration>(clock.now().time_since_epoch());

					// Calculate time left
					timeLeft = timeLeft - (end - start);

					std::this_thread::sleep_for(timeLeft);
				}
				});
		}

		inline void Stop() {
			active = false;
		}

	private:
		std::thread thread;
	};

*/
	// ----------------------------------------------------------------
		template<typename Duration, typename ActionRet, typename...ActionArgs>
	class Timer {
	public:
		Duration deltaTime = Duration(0);
		int interval;
		bool active = false;
		Duration timeLeft;
		Duration waitTime;
		
		Action<ActionRet, ActionArgs...> OnTick;
		
		
		/// <param name="interval">in Duration units</param>
		inline Timer(int interval = 1) : OnTick(obj) {
			this->interval = interval;
			this->timeLeft = Duration(interval);
			this->waitTime = Duration(interval);
		}

		template<typename...EventArgs>
		inline void Start(int interval, EventArgs...args) {
			if (active)
				return;

			auto clock = std::chrono::steady_clock();
			this->interval = interval;
			this->timeLeft = Duration(interval);
			this->waitTime = timeLeft;
			this->active = true;

			this->thread = new std::thread([&]() {
				active = true;
				while (this->active) {
					timeLeft = waitTime;

					// Do OnTick and mark timepoints
					auto start = std::chrono::duration_cast<Duration>(clock.now().time_since_epoch());
				    OnTick();
					auto end = std::chrono::duration_cast<Duration>(clock.now().time_since_epoch());
					this->deltaTime = end - start;

					// Calculate time left
					timeLeft = timeLeft - (end - start);
					if (timeLeft < Duration(0)) {
						timeLeft += waitTime;
					}

					std::this_thread::sleep_for(timeLeft);
				}
				});
		}

		inline void Stop() {
			if (active)
				active = false;
		}

		inline std::thread::id GetThreadId() {
			return this->thread->get_id();
		}
	private:
		std::thread* thread = new std::thread();
	};

}


	template<typename Duration, class ActionObjectClass, typename ActionRet, typename...ActionArgs>
	class Timer {
	public:
		Duration deltaTime = Duration(0);
		int interval;
		bool active = false;
		Duration timeLeft;
		Duration waitTime;
		
		Action<ActionObjectClass, ActionRet, ActionArgs...> OnTick;
		
		
		/// <param name="interval">in Duration units</param>
		inline Timer(ActionObjectClass& obj, int interval = 1) : OnTick(obj) {
			this->interval = interval;
			this->timeLeft = Duration(interval);
			this->waitTime = Duration(interval);
		}

		template<typename...EventArgs>
		inline void Start(int interval, EventArgs...args) {
			if (active)
				return;

			auto clock = std::chrono::steady_clock();
			this->interval = interval;
			this->timeLeft = Duration(interval);
			this->waitTime = timeLeft;
			this->active = true;

			this->thread = new std::thread([&]() {
				active = true;
				while (this->active) {
					timeLeft = waitTime;

					// Do OnTick and mark timepoints
					auto start = std::chrono::duration_cast<Duration>(clock.now().time_since_epoch());
				    OnTick();
					auto end = std::chrono::duration_cast<Duration>(clock.now().time_since_epoch());
					this->deltaTime = end - start;

					// Calculate time left
					timeLeft = timeLeft - (end - start);
					if (timeLeft < Duration(0)) {
						timeLeft += waitTime;
					}

					std::this_thread::sleep_for(timeLeft);
				}
				});
		}

		inline void Stop() {
			if (active)
				active = false;
		}

		inline std::thread::id GetThreadId() {
			return this->thread->get_id();
		}
	private:
		std::thread* thread = new std::thread();
	};

}
