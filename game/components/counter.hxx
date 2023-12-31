#pragma once

#include <array>
#include <memory>
#include <vector>

#include "engine.hxx"
#include "number.hxx"

// if you need to use this class - first of all use set_quantity_of_digits(.,.)
// after then init()
class counter {
public:
    enum class sign { sign,
        unsign };

    void set_quantity_of_digits(const size_t&, const sign&);

    bool init();
    void update();
    void draw();

    void set_displayed_number(const size_t&);
    bool is_overflow(const size_t&);

    void set_vertexes(const float& x, const float& y, const float& w, const float& h);
    void set_color(const grottans::color&);
    void set_vertex_buffer();
    void set_hide_zeros(const bool&);

    counter(grottans::engine* e)
        : engine{ e }
        , quantity_of_digits{ 0 }
        , number_on_screen{ 0 }
        , score_numbers{ 0 }
        , hide_zeros{ true }
        , m_sign{ sign::unsign }
    {
    }

    ~counter() = default;
    counter() = delete;
    counter(const counter&) = delete;
    const counter& operator=(const counter&) = delete;
    counter(counter&&) = delete;
    counter& operator=(counter&&) = delete;

private:
    grottans::engine* engine;

    size_t quantity_of_digits;
    size_t number_on_screen;

    std::array<size_t, 6> score_numbers;

    bool hide_zeros;

    sign m_sign;

    std::vector<std::unique_ptr<number> > digits;
};
