#pragma once
#include <string>
#include <Windows.h>
#include <sstream>

namespace Cinnamon {

	inline std::wstringstream& operator <<(std::wstringstream& ws, COORD c) {
		ws << "(" << c.X << ", " << c.Y << ")";
		return ws;
	}

}
