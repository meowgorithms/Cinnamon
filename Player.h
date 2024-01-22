#pragma once
#include <memory>
#include "engine.h"

namespace Cinnamon {

	class Player : public GameObject {
	public:
		
		int health = 100;

		Vector2 velocity { 0, 0 };

		Timer<std::chrono::seconds, Player, void> logTimer;


		Action<Player, void> OnDeath = Action<Player, void>(*this);
		

		inline Player() : GameObject(), logTimer(*this, 1) {
			logTimer.OnTick += &Player::LogVelocity;
			OnDeath += &Player::LogVelocity;
		}

		inline void Start() override {
			logTimer.Start(1);
		}

		inline void Update() override {
			if (health < 100) {
				OnDeath();
			}
		}

		inline void FixedUpdate() override {
			if (Input::KeyDown('W')) {
				velocity = { 0 , 1 };
			}

			if (Input::KeyDown('A')) {
				velocity = { -1, 0 };
			}

			if (Input::KeyDown('D')) {
				velocity = { 1, 0 };
			}

			if (Input::KeyDown('S')) {
				velocity = { 0, -1 };
			}
			if (position.x < -100) {
				health -= Game::Instance().fixedDeltaTime.count() / 1000. * 1.;
			}
			position += velocity;
		}

		inline void LogVelocity() {
			std::cout << "YOOOO WE NYOOMIN AT MACH (" << position.x << ", " << position.y << ")\n";
			std::cout << "Player health: " << health << "\n";
			std::cout << "dt: " << Game::Instance().deltaTime.count() << "\n";
			std::cout << "fdt: " << Game::Instance().fixedDeltaTime.count() << "\n";
		}

		inline void Death() {
			std::cout << "ONOOOO I DIE\n";

		}
	};

}
