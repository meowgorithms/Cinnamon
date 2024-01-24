#pragma once
#include <chrono>
#include "timers.h"
#include "level.h"
#include "display.h"
#include "CinnamonObject.h"

namespace Cinnamon {
	class Game {
	public:
		Timer<std::chrono::microseconds, Game, void> fixedUpdateTimer;
		Level activeLevel;
		//std::unordered_map<std::string, Level>		LevelMap;

		Screen display;
		// This is in milliseconds
		std::chrono::microseconds deltaTime{ 0 };
		std::chrono::microseconds fixedDeltaTime = fixedUpdateTimer.deltaTime;

		double deltaTimeSeconds{ 0 };
		double fixedDeltaTimeSeconds{ 0 };

		std::thread updateThread;

		inline static Game& Instance() {
			static Game game;
			return game;
		}

		inline void InitializeGame(Cinnamon::Level* activeLevel, int fixedUpdateInterval) {
			Game::Instance().activeLevel = *activeLevel;
			Game::Instance().fixedUpdateTimer.interval = fixedUpdateInterval;
		}

		inline void InitializeGame(Cinnamon::Level& activeLevel, int fixedUpdateInterval) {
			Game::Instance().activeLevel = activeLevel;
			Game::Instance().fixedUpdateTimer.interval = fixedUpdateInterval;
		}


		inline void InitializeLevel() {
			for (size_t i = 0; i < activeLevel.gameObjects.size(); i++)
			{
				activeLevel.gameObjects[i]->Start();
			}
		}

		inline void Update() {
			updateThread = std::thread([&]() {
				while (true) {
					auto start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
					for (size_t i = 0; i < activeLevel.gameObjects.size(); i++) {
						activeLevel.gameObjects[i]->Update();
						//std::cout << "Updating...\n";
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
					auto end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
					deltaTime = (end - start);
					deltaTimeSeconds = deltaTime.count() / 1'000'000.;
				}
				});
		}

		inline void FixedUpdate() {
			for (size_t i = 0; i < activeLevel.gameObjects.size(); i++)
			{
				activeLevel.gameObjects[i]->FixedUpdate();
				//std::cout << "Doing Fixed Update...\n";
			}
			fixedDeltaTime = fixedUpdateTimer.deltaTime;
			fixedDeltaTimeSeconds = fixedDeltaTime.count() / 1'000'000.;
		}

		inline void StartLevel() {
			InitializeLevel();
			fixedUpdateTimer.Start(fixedUpdateTimer.interval);
			Update();
		}

	protected:
		inline Game() : fixedUpdateTimer(*this, 16'000) {
			fixedUpdateTimer.OnTick += &Game::FixedUpdate;
		};

	private:
		inline Game(const Game&) = delete;
		inline Game(Game&&) = delete;
		inline void operator =(const Game&) = delete;
		inline void operator =(Game&&) = delete;
	};

}
