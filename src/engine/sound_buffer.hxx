#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <memory>
#include <string_view>

namespace grottans {

class sound_buffer {
public:
    enum class properties {
        once,
        looped
    };

    virtual ~sound_buffer();
    virtual void play(const properties&) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief The sound_buffer_impl class
///
class sound_buffer_impl : public sound_buffer {
public:
    sound_buffer_impl(std::string_view path, SDL_AudioDeviceID device,
        SDL_AudioSpec audio_spec);
    ~sound_buffer_impl() final;

    void play(const properties& prop) final
    {
        // Lock callback function
        SDL_LockAudioDevice(device);

        // here we can change properties
        // of sound and dont collade with multithreaded playing
        current_index = 0;
        is_playing = true;
        is_looped = (prop == properties::looped);

        // unlock callback for continue mixing of audio
        SDL_UnlockAudioDevice(device);
    }

    std::unique_ptr<uint8_t[]> tmp_buf;
    uint8_t* buffer;
    uint32_t length;
    uint32_t current_index = 0;
    SDL_AudioDeviceID device;
    bool is_playing = false;
    bool is_looped = false;
};

///////////////////////////////////////////////////////////////////////////////
static std::string_view get_sound_format_name(uint16_t format_value)
{
    static const std::map<int, std::string_view> format = {
        { AUDIO_U8, "AUDIO_U8" }, { AUDIO_S8, "AUDIO_S8" },
        { AUDIO_U16LSB, "AUDIO_U16LSB" }, { AUDIO_S16LSB, "AUDIO_S16LSB" },
        { AUDIO_U16MSB, "AUDIO_U16MSB" }, { AUDIO_S16MSB, "AUDIO_S16MSB" },
        { AUDIO_S32LSB, "AUDIO_S32LSB" }, { AUDIO_S32MSB, "AUDIO_S32MSB" },
        { AUDIO_F32LSB, "AUDIO_F32LSB" }, { AUDIO_F32MSB, "AUDIO_F32MSB" },
    };

    auto it = format.find(format_value);
    return it->second;
}

static std::size_t get_sound_format_size(uint16_t format_value)
{
    static const std::map<int, std::size_t> format = {
        { AUDIO_U8, 1 }, { AUDIO_S8, 1 }, { AUDIO_U16LSB, 2 },
        { AUDIO_S16LSB, 2 }, { AUDIO_U16MSB, 2 }, { AUDIO_S16MSB, 2 },
        { AUDIO_S32LSB, 4 }, { AUDIO_S32MSB, 4 }, { AUDIO_F32LSB, 4 },
        { AUDIO_F32MSB, 4 },
    };

    auto it = format.find(format_value);
    return it->second;
}

} //end of namespace
