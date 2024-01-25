#pragma once
#include "Screen.h"
#include "Debug.h"

namespace Cinnamon {
	class Camera {
	public:
		inline Camera() {};
		
		// Stages renderable buffer into screen -- does NOT render
		//inline void Capture(Screen& screen, Level& level) {
			
		//}

		// Stages renderable buffer into screen centered around obj world position -- does NOT render
		inline void Capture(Screen& screen, Level& level) {
			for (size_t i = 0; i < level.gameObjects.size(); i++) {
				auto& obj = level.gameObjects[i];
				if (obj->layer == 0) {
					//DebugLog("Layer: ", obj.layer, "\n");
					//DebugLog("WorldPos: ", obj.GetWorldPosition().X, ", ", obj.GetWorldPosition().Y, "\n");
					screen.PlaceAt(obj->displayChar, obj->GetWorldPosition());
				}
			}
		}

		inline int GetScreenIndexFromWorldPos(Screen& screen, Level& level, COORD worldPos) {

		}
	};

}

