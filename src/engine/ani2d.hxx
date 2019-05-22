#pragma once

#include <vector>

#include "./engine.hxx"
//#include "./sprite.hxx"

class ani2d {
public:
    ani2d();

    //void sprites(const std::vector<sprite>& sprites_values) { sprites_ = sprites_values; }

    float get_fps() const { return fps; }
    void set_fps(float fps_value) { fps = fps_value; }

    void restart() { current_time_ = 0.f; }

    void set_uv_coord(grottans::engine& e, float delta_time);

private:
    std::vector<sprite> sprites_;

    float fps;

    float current_time_ = 0.f;
};
