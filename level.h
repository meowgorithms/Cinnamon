#pragma once
#include <Windows.h>
#include "vector"
#include "CinnamonObject.h"


namespace Cinnamon {
	class Level {
	public:
		std::vector<GameObject*> gameObjects = std::vector<GameObject*>();
		COORD size { 0, 0 };
	};
}


