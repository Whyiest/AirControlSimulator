#ifndef CONTROLEAERIEN_GROUPE_2_G_DISRUPTION_H
#define CONTROLEAERIEN_GROUPE_2_G_DISRUPTION_H

#include "Libraries.h"
#include "util.h"

class Disruption {

private:

    Pair m_pos;

    int m_power;

public:

    Disruption(Pair Pos, int Power);

    ~Disruption();

    void update();

    void display(sf::RectangleShape &rect, sf::RenderWindow &window, bool debug);

    Pair getPos();

    int getPower() const;
};


#endif
