#include <utility>


#include "../HEADERS/Disruption.h"

Disruption::Disruption(Pair Pos, int Power) : m_pos(std::move(Pos)), m_power(Power) {}

Disruption::~Disruption() = default;

void Disruption::update() {
    std::vector<int> offsets = {-1, 0, 1};

    m_pos.second += offsets[randomInt(0, offsets.size() - 1)];
    if (m_pos.second < 0) m_pos.second = GRID_HEIGHT - 1;
    else if (m_pos.second > GRID_HEIGHT - 1) m_pos.second = 0;

    if (m_pos.second < GRID_HEIGHT / 2) m_pos.first += 1;
    else m_pos.first -= 1;
    if (m_pos.first < 0) m_pos.first = GRID_WIDTH - 1;
    else if (m_pos.first > GRID_WIDTH - 1) m_pos.first = 0;

    m_power += offsets[randomInt(0, offsets.size() - 1)];
    if (m_power <= 0) m_power = 1;
    else if (m_power > CELL_SIZE / 2) m_power = CELL_SIZE / 2;
}

void Disruption::display(sf::RectangleShape &rect, sf::RenderWindow &window, bool debug) {
    if (debug) {
        rect.setFillColor({255, 0, 255, 155});
        rect.setPosition(m_pos.first * CELL_SIZE, m_pos.second * CELL_SIZE);
        window.draw(rect);
    }
}

Pair Disruption::getPos() {
    return m_pos;
}

int Disruption::getPower() const {
    return m_power;
}