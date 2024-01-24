#pragma once
#include <Windows.h>
#include "vector"

namespace Cinnamon {
	class GameObject;

	class Level {
	public:
		std::vector<GameObject*> gameObjects = std::vector<GameObject*>();
		COORD size = { 0, 0 };

		inline void Start() {

		}
	};
}


