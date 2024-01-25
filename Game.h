#pragma once
#include <chrono>
#include "timers.h"
#include "level.h"
#include "Screen.h"
#include "CinnamonObject.h"
#include "Camera.h"
#include "Debug.h"

#define SIXTY_TICKS_PER_SECOND 16'666
#define MICROSECONDS_IN_SECOND 1'000'000

namespace Cinnamon {
	class Game {
	public:
		Level activeLevel;
		//std::unordered_map<std::string, Level>		LevelMap;

		Screen screen;
		Camera activeCamera;

		std::chrono::microseconds deltaTime { 0 };
		std::chrono::microseconds fixedDeltaTime { SIXTY_TICKS_PER_SECOND };

		double deltaTimeSeconds{ 0 };

		std::thread fixedUpdateThread;
		double fixedDeltaTimeMicroseconds = double(SIXTY_TICKS_PER_SECOND);
		double fixedDeltaTimeSeconds{ fixedDeltaTimeMicroseconds / double(MICROSECONDS_IN_SECOND) };
		Stopwatch<std::chrono::microseconds> stopwatch;

		inline static Game& Instance() {
			static Game game;
			return game;
		}

		inline void InitializeGame(Cinnamon::Level* activeLevel, int fixedDeltaTime) {
			Game::Instance().activeLevel = *activeLevel;
			Game::Instance().fixedDeltaTime = std::chrono::microseconds(fixedDeltaTime);
			Game::Instance().fixedDeltaTimeMicroseconds = fixedDeltaTime;
			Game::Instance().fixedDeltaTimeSeconds = fixedDeltaTime / double(MICROSECONDS_IN_SECOND);
			Game::Instance().InitializeLevel();
		}

		inline void InitializeGame(Cinnamon::Level& activeLevel, int fixedDeltaTime) {
			Game::Instance().activeLevel = activeLevel;
			Game::Instance().fixedDeltaTime = std::chrono::microseconds(fixedDeltaTime);
			Game::Instance().fixedDeltaTimeMicroseconds = fixedDeltaTime;
			Game::Instance().fixedDeltaTimeSeconds = fixedDeltaTime / double(MICROSECONDS_IN_SECOND);
			Game::Instance().InitializeLevel();
		}

		inline void InitializeLevel() {
			for (size_t i = 0; i < activeLevel.gameObjects.size(); i++) {
				activeLevel.gameObjects[i]->Start();
			}
		}

		inline void Update() {
			while (true) {
				auto start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
				for (size_t i = 0; i < activeLevel.gameObjects.size(); i++) {
					activeLevel.gameObjects[i]->mut.lock();
					activeLevel.gameObjects[i]->Update();
					activeLevel.gameObjects[i]->mut.unlock();
				}

				Render();
				auto end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
				deltaTime = (end - start);
				deltaTimeSeconds = deltaTime.count() / double(MICROSECONDS_IN_SECOND);
			}
		}

		inline void FixedUpdate() {
			fixedUpdateThread = std::thread([&, this]() {
				stopwatch.Start();
				for (size_t i = 0; i < activeLevel.gameObjects.size(); i++) {
					activeLevel.gameObjects[i]->mut.lock();
					activeLevel.gameObjects[i]->FixedUpdate();
					activeLevel.gameObjects[i]->mut.unlock();
				}
				auto fdt = stopwatch.Lap();
				std::this_thread::sleep_for(std::chrono::microseconds(int(fixedDeltaTimeMicroseconds)) - fdt);
			});
		}

		inline void StartLevel() {
			InitializeLevel();
			FixedUpdate();
			Update();
		}

	protected:
		inline Game() {

		};

		inline void Render() {
			// Anything on layer 0 gets put in the display buffer at its world position to screen position location
			activeCamera.Capture(screen, activeLevel);
			RenderScreen(screen);
		}

	private:
		inline Game(const Game&) = delete;
		inline Game(Game&&) = delete;
		inline void operator =(const Game&) = delete;
		inline void operator =(Game&&) = delete;
	};

}
