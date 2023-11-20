#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>

#include "./Libs/Public/imgui.h"
#include "./Libs/Public/imgui_internal.h"

extern float accent_colour[4];

namespace custom {
    bool selected(const char* label, bool tab = false);
}
