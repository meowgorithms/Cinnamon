#pragma once
#include <memory>
#include "engine.h"
#include <mutex>

namespace Cinnamon {

	class Player : public GameObject {
	public:

		int health = 100;
		Vector2 velocity;
		double velScale = 2.;
		Action<Player, void> OnDeath = Action<Player, void>(*this);
		

		inline Player() : GameObject() {
			displayChar = L'O';	
		}

		inline void Start() override {
		}

		inline void Update() override {
			if (health < 100) {
				OnDeath();
			}
			velocity = { 0, 0 };

			if (Input::KeyDown('W')) {
				velocity += { 0 , -1 };
			}

			if (Input::KeyDown('A')) {
				velocity += { -1, 0 };
			}

			if (Input::KeyDown('D')) {
				velocity += { 1, 0 };
			}

			if (Input::KeyDown('S')) {
				velocity += { 0, 1 };
			}
				
			if (Input::KeyDown(VK_LBUTTON)) {
				DebugLog("Left Mouse Down");
			}

			if (Input::KeyDown(VK_RBUTTON)) {
				DebugLog("Right Mouse Down");
			}

			position += velocity * Game::Instance().deltaTimeSeconds * velScale;

		}

		inline void FixedUpdate() override {
		}

		inline void Log(int x) {
			DebugLog("Player Position: ", position);
			DebugLog("Delta Time: ", Game::Instance().deltaTimeSeconds);
			DebugLog("Fixed Delta Time: ", Game::Instance().fixedDeltaTimeSeconds);
			
		}
	};

}
