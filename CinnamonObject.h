#pragma once
#include "vector2.h"
#include <Windows.h>
#include <math.h>
#include <mutex>

namespace Cinnamon {
	class GameObject {
	public:
		std::mutex mut;
		Vector2	position;
		int layer = 0;
		wchar_t displayChar = L' ';

		inline GameObject()
			: position{ 0, 0 },
			layer(0) {};

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


		COORD GetWorldPosition() {
			return { SHORT(floor(position.x)), SHORT(floor(position.y)) };
		}
	};
}
