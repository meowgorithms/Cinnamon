#pragma once
#include <iostream>
namespace Cinnamon {
	struct Vector2 {
	public:
		double x;
		double y;

		Vector2() : x(0), y(0) {}
		Vector2(double x, double y) : x(x), y(y) {}


		//  Vector2 op= operators

		inline void operator += (Vector2 other) {
			this->x += other.x;
			this->y += other.y;
		}

		inline void operator += (double d) {
			this->x += d;
			this->y += d;
		}

		inline void operator -=(Vector2 other) {
			this->x -= other.x;
			this->y -= other.y;
		}

		inline void operator -=(double d) {
			this->x -= d;
			this->y -= d;
		}

		// Basic operators
		inline Vector2 operator +(Vector2 other) {
			return Vector2{ x + other.x, y + other.y };
		}

		inline Vector2 operator -(Vector2 other) {
			return Vector2{ x - other.x, y - other.y };
		}

		// Returns the dot product of the vectors
		inline double operator *(Vector2 other) {
			return (x * other.x + y * other.y);
		}

		// Scaler multiplication
		inline Vector2 operator *(double d) {
			return Vector2 { x * d, y * d };
		}

		// Place-wise multiplication
		inline Vector2 Hadamard(Vector2 other) {
			return Vector2{ x * other.x, y * other.y };
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
