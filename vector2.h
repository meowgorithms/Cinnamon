#pragma once

namespace Cinnamon {
	struct Vector2 {
	public:
		double x;
		double y;

		Vector2() : x(0), y(0) {}
		Vector2(double x, double y) : x(x), y(y) {}

		inline void operator += (Vector2 other) {
			this->x += other.x;
			this->y += other.y;
		}

		inline void operator += (double d) {
			this->x += d;
			this->y += d;
		}

		inline Vector2& operator * (double d) {
			this->x *= d;
			this->y *= d;
			return *this;
		}
	};
}
