#include "./counter.hxx"

bool counter::init(grottans::engine* engine)
{
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->init(engine);
    }
    return EXIT_SUCCESS;
}

void counter::set_quantity_of_digits(const size_t& t, sign s_)
{
    size_t quantity = t;
    if (t > 5) {
        quantity_of_digits = 0;
        std::cerr << "error: too much number of the digits" << std::endl;
        throw std::runtime_error("can't start counter");
    }

    if (s_ == sign::sign) {
        quantity++;
    }

    quantity_of_digits = quantity;

    ///creating new numbers
    for (size_t i = 0; i < quantity; i++) {
        digits.push_back(std::unique_ptr<number>(new number));
    }
}

void counter::set_vertexes(float x, float y, float w, float h)
{
    //w=h=0.08f in previous version of the game
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->set_vertexes(x + w * i, y, w, h);
    }
}

void counter::set_color(grottans::color col)
{
    if (col.get_r() + col.get_b() + col.get_a() + col.get_a() > 1020) {
        std::cerr << "error: counter: incorrect color" << std::endl;
        throw std::runtime_error("incorrect color");
    }
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->set_color(col);
    }
}

void counter::set_vertex_buffer(grottans::engine* engine)
{
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->set_vertex_buffer(engine);
    }
}

void counter::set_displayed_number(const size_t& t)
{
    if (t == 0) {
        for (size_t i = 0; i < quantity_of_digits; i++) {
            digits[i]->set_number_and_texture(0);
        }
        return;
    }

    size_t tmp = t;
    for (int i = quantity_of_digits - 1; i >= 0; i--) {
        score_numbers[i] = tmp % 10;

        ///set digits and textures
        digits[i]->set_number_and_texture(score_numbers[i]);

        tmp = tmp / 10;
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

    if (!result) {
        std::cerr << "error: counter overflow" << std::endl;
        throw std::runtime_error("counter overflow");
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void counter::draw(grottans::engine* engine)
{
    for (size_t i = 0; i < quantity_of_digits; i++) {
        digits[i]->draw(engine);
    }
}
