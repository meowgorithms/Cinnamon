#pragma once
#include <iostream>
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

		inline Vector2 operator * (double d) {
			return Vector2 { x * d, y * d };
		}

		inline friend std::ostream& operator <<(std::ostream& os, Vector2 v) {
			os << "(" << v.x << ", " << v.y << ")";
			return os;
		}

		inline friend std::wstringstream& operator <<(std::wstringstream& ws, Vector2 v) {
			ws << "(" << v.x << ", " << v.y << ")";
			return ws;
		}
	};
}
