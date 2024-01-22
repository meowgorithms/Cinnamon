#pragma once

namespace Cinnamon {
	struct Vector2 {
	public:
		double x;
		double y;

		Vector2() : x(0), y(0) {}
		Vector2(double x, double y) : x(x), y(y) {}

		inline void operator += (Vector2 other) {
			x += other.x;
			y += other.y;
		}
	};
}
