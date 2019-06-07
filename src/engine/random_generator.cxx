#include "./random_generator.hxx"

std::mt19937& random_generator::getMt19937()
{
    return random_generator::instance().mMt;
}
