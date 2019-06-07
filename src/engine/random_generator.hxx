#pragma once

#include <chrono>
#include <random>

class random_generator {
public:
    static std::mt19937& getMt19937();

private:
    random_generator()
    {
        std::random_device rd;

        if (rd.entropy() != 0) {
            std::seed_seq seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
            mMt.seed(seed);
        } else {
            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            mMt.seed(seed);
        }
    }

    ~random_generator() {}

    static random_generator& instance()
    {
        static random_generator s;
        return s;
    }
    random_generator(random_generator const&) = delete;
    random_generator& operator=(random_generator const&) = delete;

    std::mt19937 mMt;
};
