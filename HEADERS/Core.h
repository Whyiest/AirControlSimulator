#ifndef CONTROLEAERIEN_GROUPE_2_G_CORE_H
#define CONTROLEAERIEN_GROUPE_2_G_CORE_H

#include "Libraries.h"
#include "util.h"
#include "Network.h"

class Core {

private:

    sf::RenderWindow m_window;

    sf::Texture backgroundTexture;

    sf::Sprite backgroundSprite;

    Pair m_mouseGridPos;

    Network m_network;

    bool m_debug = true;

    int utPassed = 0;

public:

    Core();

    ~Core();

    void loop(bool doUpdate);

    void handleEvents();

    void display();

    void initialisation();

    void createAircraft();

    FlightsPlanning* createFP(Aircraft* withThis);
};

#endif
