#include "./random_generator.hxx"

std::mt19937& random_generator::getMt19937()
{
    /* // internal random generator from Intel CPU
    unsigned long long ran = 0;
    __builtin_ia32_rdrand64_step(&ran);
    */
    return random_generator::instance().mMt;
}
