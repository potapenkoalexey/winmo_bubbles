#include "counter.hxx"

//counter::~counter()
//{
//}

bool counter::init()
{
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->init();
    }
    return EXIT_SUCCESS;
}

void counter::set_quantity_of_digits(const size_t& t, const sign& s_)
{
    size_t quantity = t;
    if (t > 5) {
        quantity_of_digits = 0;
        std::cerr << __FILE__ << __LINE__ << "error: too much number of the digits" << std::endl;
        throw std::runtime_error("can't start counter");
    }

    if (s_ == sign::sign) {
        quantity++;
        m_sign = sign::sign;
    }

    quantity_of_digits = quantity;

    /// creating new numbers
    for (size_t i = 0; i < quantity; i++) {
        digits.push_back(std::make_unique<number>(engine));
    }
}

void counter::set_vertexes(const float& x, const float& y, const float& w,
    const float& h)
{
    // w=h=0.08f in previous version of the game
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->set_vertexes(x + w * i, y, w, h);
    }
}

void counter::set_color(const grottans::color& col)
{
    if (col.get_r() + col.get_b() + col.get_a() + col.get_a() > 1020) {
        std::cerr << "error: counter: incorrect color" << std::endl;
        throw std::runtime_error("incorrect color");
    }
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->set_color(col);
    }
}

void counter::set_vertex_buffer()
{
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->set_vertex_buffer();
    }
}

void counter::set_hide_zeros(const bool& temp) { hide_zeros = temp; }

void counter::set_displayed_number(const size_t& t)
{
    if (is_overflow(t) == false) {
        number_on_screen = t;
    }

    if (t == 0) {
        for (size_t i = 0; i < quantity_of_digits; i++) {
            digits[i]->set_number_and_texture(0);
        }
        return;
    }

    size_t tmp = t;
    for (int i = quantity_of_digits - 1; i >= 0; i--) {
        score_numbers[i] = tmp % 10;

        /// set digits and textures
        digits[i]->set_number_and_texture(score_numbers[i]);

        tmp = tmp / 10;
    }

    if (m_sign == sign::sign) {
        digits[0]->set_number_and_texture(10);
    }
}

bool counter::is_overflow(const size_t& t)
{
    bool result = false;
    if (quantity_of_digits == 1) {
        if (t > 9)
            result = true;
    }
    if (quantity_of_digits == 2) {
        if (t > 99)
            result = true;
    }
    if (quantity_of_digits == 3) {
        if (t > 999)
            result = true;
    }
    if (quantity_of_digits == 4) {
        if (t > 9999)
            result = true;
    }
    if (quantity_of_digits == 5) {
        if (t > 99999)
            result = true;
    }

    if (result) {
        std::cerr << __FILE__ << __LINE__ << "error: counter overflow (" << t << ")" << std::endl;
        throw std::runtime_error("counter overflow");
    }
    return false;
}

void counter::draw()
{
    if (0 == quantity_of_digits || 0 == number_on_screen)
        return;

    if (hide_zeros) {
        if (number_on_screen < 10) {
            digits[quantity_of_digits - 1]->draw();
            return;
        }
        if (number_on_screen < 100) {
            digits[quantity_of_digits - 1]->draw();
            digits[quantity_of_digits - 2]->draw();
            return;
        }

        if (number_on_screen < 1000) {
            digits[quantity_of_digits - 1]->draw();
            digits[quantity_of_digits - 2]->draw();
            digits[quantity_of_digits - 3]->draw();
            return;
        }

        if (number_on_screen < 10000) {
            digits[quantity_of_digits - 1]->draw();
            digits[quantity_of_digits - 2]->draw();
            digits[quantity_of_digits - 3]->draw();
            digits[quantity_of_digits - 4]->draw();
            return;
        }

        if (number_on_screen < 100000) {
            digits[quantity_of_digits - 1]->draw();
            digits[quantity_of_digits - 2]->draw();
            digits[quantity_of_digits - 3]->draw();
            digits[quantity_of_digits - 4]->draw();
            digits[quantity_of_digits - 5]->draw();
            return;
        }
    } else {

        for (int i = quantity_of_digits - 1; i >= 0; i--) {
            digits[i]->draw();
        }
    }
}
