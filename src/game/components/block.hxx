#pragma once

#include "../../../src/engine/engine.hxx"
#include "./../global_variables.hxx"

/*!
\file
\class 'block' class description

this class is base for all graphical elements
*/
class block {
public:
    /// set of block colors
    enum class palette {
        non,
        red,
        blue,
        bomb,
        green,
        black,
        purple,
        yellow
    };

    /// set of block states
    enum class block_state {
        fixed, ///< indicates that block is stationary
        fliping_over, ///< indicates that block is over of other blocks on render
        fliping_under, ///< indicates that block is under of other blocks on render
        falling, ///< indicates that there isn't block under this block and it can fall
        shifting, ///< indicates that there isn't block in left of this block
        disappearing ///< indicates that block should disappear
    };

    /// used for indicating flipping direction
    enum class block_direction {
        right,
        down,
        left,
        up,
        non
    };

    /*! \brief return current FPS value*/
    float get_fps() const;

    /*! \brief set new FPS value*/
    void set_fps(const float& fps_value);

    /*! \brief update UV-coordinates of the block every frame
        \param[in] arr_uv_buf array of the UV-coordinates on texture
        \param[in] delta_time global timestamp between frames
    */
    void update_uv_coord(
        const std::array<grottans::tri2, 32>& arr_uv_buf,
        const milli_sec& delta_time);

    /*! \brief restoring primary parameters of the block
        \param[in] arr_uv_buf array with UV coordinates on texture
    */
    void restore_original_parameters(
        const std::array<grottans::tri2, 32>& arr_uv_buf);

    /*! \brief set random color in classic mode
    */
    void get_random_color_from_classic();
    /*! \brief set primary random color in extreme mode
    */
    void get_random_color_from_extreme();
    /*! \brief set random color in extreme mode
    */
    void get_random_color_from_extreme_with_bomb();

    /*! \brief block rendering
    */
    void draw();

    /*! \brief constructor*/
    block(grottans::engine* e)
        : color{ palette::black }
        , state{ block_state::fixed }
        , flip_direction{ block::block_direction::non }
        , selected{ false }
        , visible{ true }
        , motion{ false }
        , position{ 0.f, 0.f }
        , move{ grottans::mat2x3::scale(1.f, 1.f) }
        , aspect{ grottans::mat2x3::scale(1.f, 1.f) }
        , texture{ nullptr }
        , v_buf{ nullptr }
        , fps{ g_FPS }
        , current_time{ 0.f }
        , falling_frame_index{ 0 }
        , shifting_frame_index{ 0 }
        , fliping_frame_index{ 0 }
        , engine{ e }
    {
        move.delta.x = 0;
        move.delta.y = 0;
    }
    ~block() = default;
    block() = delete;
    block(const block&) = delete;
    const block& operator=(const block&) = delete;
    block(block&&) = delete;
    block& operator=(block&&) = delete;

    ///////////////////////////////////////////////////////////////////////////
    palette color;
    block_state state;
    block_direction flip_direction;

    bool selected;
    bool visible;
    bool motion;
    grottans::vec2 position;
    grottans::mat2x3 move;
    grottans::mat2x3 aspect;
    grottans::texture* texture;
    grottans::vertex_buffer* v_buf;

    ///for animation
    float fps;
    float current_time;
    std::array<grottans::tri2, 2> tr_disappear;

    size_t falling_frame_index;
    size_t shifting_frame_index;
    size_t fliping_frame_index;

private:
    grottans::engine* engine;
};
