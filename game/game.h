#pragma once

class game final {
public:

static game* instance(){ return &m_game; }

    ~game();
    game(game const&) = delete;
    game& operator=(game const&) = delete;
    game(game&&) = delete;
    game& operator=(game&&) = delete;

protected:
    game(){}

private:
    static game m_game;
};