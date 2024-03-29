#pragma once

#include <iostream>
#include <string>
#include "utils.h"

namespace Cinnamon {

    template<class...T>
    void DebugLog(T...msg) {
        std::wstringstream wss;
        ((wss << msg), ...);
        wss << "\n";
        OutputDebugString(wss.str().c_str());
    }

}
