#include "./counter.hxx"

void counter::set_number_of_digit(const size_t& num)
{
    if (num > 6) {
        quantity_of_digits = 0;
        std::cerr << "error: too much number of the digits" << std::endl;
        throw std::runtime_error("can't start counter");
    }
    quantity_of_digits = num;
}

void counter::set_displayed_number(const size_t& number)
{
    //is_overflow(number);
}

bool counter::is_overflow(const size_t& number)
{
    if (number > (quantity_of_digits /*?????????????*/)) {
        std::cerr << "error: counter overflow" << std::endl;
        throw std::runtime_error("counter overflow");
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void counter::draw(grottans::engine*)
{
    if (quantity_of_digits == 0) {
        return;
    }
}
