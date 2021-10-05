#pragma once

#include <memory>

#include "engine.hxx"
#include "../components/block.hxx"
#include "../components/field.hxx"
#include "../components/progress_desk.hxx"

class classic_state : public grottans::game_state {
public:
    bool init(grottans::engine*) override;
    void cleanup() override;

    void pause() override;
    void resume() override;

    void handle_events() override;
    void update() override;
    void draw() override;

    bool handle_mouse_event();
    void handle_start_released_event();
    void handle_left_released_event();
    void handle_right_released_event();
    void handle_up_released_event();
    void handle_down_released_event();

    static game_state* instance()
    {
        return &m_classic_state;
    }

    ~classic_state() override;
    classic_state(classic_state const&) = delete;
    classic_state& operator=(classic_state const&) = delete;

protected:
    classic_state()
        : sound_fall{ nullptr }
        , sound_destroy_big_form{ nullptr }
        , progress{ nullptr }
        , game_field{ nullptr }
    {
    }

private:
    static classic_state m_classic_state;

    grottans::sound_buffer* sound_fall;
    grottans::sound_buffer* sound_destroy_big_form;

    std::unique_ptr<progress_desk> progress;

    std::unique_ptr<field> game_field;
};
