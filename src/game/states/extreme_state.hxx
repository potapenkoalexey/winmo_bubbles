#pragma once

#include <memory>

#include "../../../src/engine/engine.hxx"
#include "../components/field.hxx"
#include "../components/progress_desk.hxx"

class extreme_state : public grottans::game_state {
public:
    bool init(grottans::engine*) override;
    void cleanup() override;

    void pause() override;
    void resume() override;

    void handle_events() override;
    void update() override;
    void draw() override;

    bool handle_mouse_pressed_event(grottans::event&);
    bool handle_mouse_released_event(grottans::event& e);
    void handle_start_released_event(const size_t& i, const size_t& j);
    void handle_left_released_event();
    void handle_right_released_event();
    void handle_up_released_event();
    void handle_down_released_event();

    static game_state* instance()
    {
        return &m_extreme_state;
    }

    ~extreme_state() override;
    extreme_state(extreme_state const&) = delete;
    extreme_state& operator=(extreme_state const&) = delete;

protected:
    extreme_state()
        : sound_fall{ nullptr }
        , sound_destroy_big_form{ nullptr }
        , sound_flip{ nullptr }
        , sound_cant_flip{ nullptr }
        , progress{ nullptr }
        , game_field{ nullptr }
    {
    }

private:
    static extreme_state m_extreme_state;

    grottans::sound_buffer* sound_fall;
    grottans::sound_buffer* sound_destroy_big_form;
    grottans::sound_buffer* sound_flip;
    grottans::sound_buffer* sound_cant_flip;

    std::unique_ptr<progress_desk> progress;

    std::unique_ptr<field> game_field;
};
