#pragma once

#include "imgui/imgui.h"
#include "menu/keybind.h"

struct Color
{
    float r, g, b, a;
    Color()
    {
        r = 1.f, g = 1.f, b = 1.f, a = 1.f;
    }
    Color(float _r, float _g, float _b, float _a)
    {
        this->r = _r;
        this->g = _g;
        this->b = _b;
        this->a = _a;
    }
    ImVec4 x()
    {
        return ImVec4(r, g, b, a);
    }
    ImU32 c()
    {
        return IM_COL32(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
    }
    ImU32 c(int m)
    {
        return IM_COL32((r * 255.f) - m, (g * 255.f) - m, (b * 255.f) - m, (a * 255.f) - m);
    }
};

namespace config
{

    inline bool no_recoil = false;
    inline bool no_spread = false;
    inline bool infinite_ammo = false;
    inline bool rapid_fire = false;

    inline float jump_height = 100.f;
    inline float player_speed = 300.f;
    inline float player_gravity = 1.f;

}