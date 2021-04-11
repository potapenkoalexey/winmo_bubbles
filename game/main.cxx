#include <memory>

#include "game.hxx"

int main(int /*argc*/, char** /*argv*/)
{
    std::unique_ptr<game> g = std::make_unique<game>();
    g->run();

    return EXIT_SUCCESS;
}
