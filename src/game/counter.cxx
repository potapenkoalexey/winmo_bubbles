#include "./counter.hxx"

void counter::set_number_of_digit(const size_t& num)
{
    if (num > 6) {
        number_of_digit = 0;
    }
    number_of_digit = num;
}

void counter::set_displayed_number(const size_t& number)
{
    if (number > 6) {
        std::cerr << "error: too much number of the digits" << std::endl;
        throw std::runtime_error("can't start counter");
    }
}

void counter::draw(grottans::engine*)
{
    if (number_of_digit == 0) {
        return;
    }
}

///////////////////////////////////////////////////////////////////////////////
void digit::draw(grottans::engine* e)
{
}
