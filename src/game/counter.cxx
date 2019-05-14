#include "./counter.hxx"

void counter::set_number_of_digit(size_t num)
{
    if (num > 6) {
    }
    number_of_digit = num;
}

void counter::set_number(size_t num)
{
    if (num > 6) {
        std::cerr << "error: too much number of the digits" << std::endl;
        throw std::runtime_error("can't start counter");
    }
}
