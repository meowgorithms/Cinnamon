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
		Event<void> OnDeath;
		const wchar_t* msg = L"Default Message";

		inline Player() : GameObject() {
			displayChar = L'O';	
		}

		inline void Start() override {
		}

		inline void Shoot() {
			DebugLog(msg);
			health -= 10;
		}

		inline void Update() override {
			if (health < 0) {
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
		
			if (Input::GetleftMouseDown()) {
				Shoot();
			}


			position += velocity * Game::Instance().deltaTimeSeconds * velScale;

		}

		inline void FixedUpdate() override {
		}

		inline void Log(int x) {
						
		}
	};

}
