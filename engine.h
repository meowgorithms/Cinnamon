#pragma once
#include <Windows.h>
#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <chrono>
#include "timers.h"
#include "event.h"
#include "input.h"
#include "display.h"
#include "vector2.h"
// A console game engine
// Module responsible for game loop, graphics, handling game objects and their loops,
// Gui, etc

namespace Cinnamon {

	class GameObject;

	class Level {
	public:
		std::vector<GameObject*> gameObjects = std::vector<GameObject*>();
		COORD size = { 0, 0 };

		inline void Start() {

		}
	private:
	};


	class GameObject {
	public:
		Vector2	position;
		COORD	worldPosition;
		int		layer;


		inline GameObject()
			: position(0, 0),
			  worldPosition { 0, 0 },
			  layer(0) {

		}

		/// <summary>
		/// Initialize object here, this happens before the start of each frame
		/// </summary>
		inline virtual void Start() {};

		/// <summary>
		/// Game logic goes here, this happens every frame
		/// </summary>
		inline virtual void Update() {};

		/// <summary>
		/// Happens every fixed update call, the interval of which is defined in the Game class, fixedUpdateInterval
		/// </summary>
		inline virtual void FixedUpdate() {};

		inline virtual ~GameObject() = default;
	};

	// Singleton boi
	class Game {
	public:
		Timer<std::chrono::microseconds, Game, void>	fixedUpdateTimer;
		Level											activeLevel;
		//std::vector<GameObject*>						GameObjects;
		//std::unordered_map<std::string, Level>		LevelMap;
		
		Screen display;
		// This is in milliseconds
		std::chrono::microseconds deltaTime { 0 };
		std::chrono::microseconds fixedDeltaTime = fixedUpdateTimer.deltaTime;

		inline static Game& Instance() {
			static Game game;
			return game;
		}

		inline void InitializeGame(Level* activeLevel, int fixedUpdateInterval) {
			Game::Instance().activeLevel = *activeLevel;
			Game::Instance().fixedUpdateTimer.interval = fixedUpdateInterval;
		}

		inline void InitializeGame(Level& activeLevel, int fixedUpdateInterval) {
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
			while (true) {
				auto start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
				for (size_t i = 0; i < activeLevel.gameObjects.size(); i++) {
					activeLevel.gameObjects[i]->Update();
					//std::cout << "Updating...\n";
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				auto end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
				deltaTime = end - start;
			}
			
		}

		inline void FixedUpdate() {
			for (size_t i = 0; i < activeLevel.gameObjects.size(); i++)
			{
				activeLevel.gameObjects[i]->FixedUpdate();
				//std::cout << "Doing Fixed Update...\n";
			}
			fixedDeltaTime = fixedUpdateTimer.deltaTime;
		}

		inline void StartLevel() {
			InitializeLevel();
			fixedUpdateTimer.Start(fixedUpdateTimer.interval);
			Update();
		}

	protected:
		inline Game() : fixedUpdateTimer(*this, 16000) {
			fixedUpdateTimer.OnTick += &Game::FixedUpdate;
		};

		inline void UpdateDeltaTime() {

		}

	private:
		inline Game(const Game&) = delete;
		inline Game(Game&&) = delete;
		inline void operator =(const Game&) = delete;
		inline void operator =(Game&&) = delete;
	};

}
