#pragma once
namespace grottans {

class sound_buffer {
public:
    enum class properties {
        once,
        looped
    };

    virtual ~sound_buffer();
    virtual void play(const properties) = 0;
};
}
