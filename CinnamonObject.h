#pragma once
#include "vector2.h"
#include <Windows.h>
#include <math.h>

namespace Cinnamon {
	class GameObject {
	public:
		Vector2	position;
		int		layer;


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


		COORD WorldPosition() {
			return { SHORT(floor(position.x)), SHORT(floor(position.y)) };
		}
	};
}
