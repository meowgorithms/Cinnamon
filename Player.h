#pragma once
#include <memory>
#include "engine.h"
#include <mutex>

namespace Cinnamon {

	class Player : public GameObject {
	public:
		
		std::mutex mut;

		int health = 100;

		Vector2 velocity { 0, 0 };
		double velScale = 10;
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
			mut.lock();
			if (health < 100) {
				OnDeath();
			}
			velocity += 1;
			mut.unlock();
		}

		inline void FixedUpdate() override {
			mut.lock();
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
				health -= Game::Instance().fixedDeltaTimeSeconds;
			}
			position += velocity * Game::Instance().fixedDeltaTimeSeconds * velScale;
			mut.unlock();
		}

		inline void LogVelocity() {
			mut.lock();
			std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")\n";
			std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
			std::cout << "Player health: " << health << "\n";
			std::cout << "dt: " << Game::Instance().deltaTimeSeconds << "\n";
			std::cout << "fdt: " << Game::Instance().fixedDeltaTimeSeconds << "\n";
			mut.unlock();
		}

		inline void Death() {
			std::cout << "ONOOOO I DIE\n";

		}
	};

}
