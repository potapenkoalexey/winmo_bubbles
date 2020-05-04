#include <algorithm>
#include <iostream>

#include "sound_buffer.hxx"

namespace grottans {

sound_buffer::~sound_buffer() {}

sound_buffer_impl::sound_buffer_impl(std::string_view path,
    SDL_AudioDeviceID device_,
    SDL_AudioSpec device_audio_spec)
    : buffer(nullptr)
    , length(0)
    , device(device_)
{
    SDL_RWops* file = SDL_RWFromFile(path.data(), "rb");
    if (file == nullptr) {
        throw std::runtime_error(std::string("can't open audio file: ") + path.data());
    }

    // freq, format, channels, and samples - used by SDL_LoadWAV_RW
    SDL_AudioSpec file_audio_spec;

    if (nullptr == SDL_LoadWAV_RW(file, 1, &file_audio_spec, &buffer, &length)) {
        throw std::runtime_error(std::string("can't load wav: ") + path.data());
    }

    std::cout << "audio format for: " << path << '\n'
              << "format: " << get_sound_format_name(file_audio_spec.format)
              << '\n'
              << "sample_size: "
              << get_sound_format_size(file_audio_spec.format) << '\n'
              << "channels: " << static_cast<uint32_t>(file_audio_spec.channels)
              << '\n'
              << "frequency: " << file_audio_spec.freq << '\n'
              << "length: " << length << '\n'
              << "time: "
              << static_cast<double>(length) / (file_audio_spec.channels * file_audio_spec.freq * get_sound_format_size(file_audio_spec.format))
              << "sec" << std::endl;

    if (file_audio_spec.channels != device_audio_spec.channels || file_audio_spec.format != device_audio_spec.format || file_audio_spec.freq != device_audio_spec.freq) {
        SDL_AudioCVT cvt;
        SDL_BuildAudioCVT(&cvt, file_audio_spec.format,
            file_audio_spec.channels, file_audio_spec.freq,
            device_audio_spec.format, device_audio_spec.channels,
            device_audio_spec.freq);
        SDL_assert(cvt.needed); // obviously, this one is always needed.
        // read your data into cvt.buf here.
        cvt.len = length;
        // we have to make buffer for inplace conversion
        tmp_buf.reset(new uint8_t[cvt.len * cvt.len_mult]);
        uint8_t* buf = tmp_buf.get();
        std::copy_n(buffer, length, buf);
        cvt.buf = buf;
        if (0 != SDL_ConvertAudio(&cvt)) {
            std::cout << "failed to convert audio from file: " << path
                      << " to audio device format" << std::endl;
        }
        // cvt.buf has cvt.len_cvt bytes of converted data now.
        SDL_FreeWAV(buffer);
        buffer = tmp_buf.get();
        length = static_cast<uint32_t>(cvt.len_cvt);
    }
}

sound_buffer_impl::~sound_buffer_impl()
{
    if (!tmp_buf) {
        SDL_FreeWAV(buffer);
    }
    buffer = nullptr;
    length = 0;
}
}
