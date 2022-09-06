#ifndef CONTROLEAERIEN_GROUPE_2_G_AIRCRAFT_H
#define CONTROLEAERIEN_GROUPE_2_G_AIRCRAFT_H

#include "Libraries.h"
#include "Cell.h"
#include "util.h"

class Aircraft {

private:

    Pair m_pos;

    int m_type; ///1 : short    2 : medium  3 : long

    sf::Texture avionTexture;

    sf::Sprite avionSprite;

    int m_consumption;

    int m_tankCapacity;

    int m_speed;

    int m_altitude; ///pour eviter les collisions

    bool m_fuite;

    bool m_crash;

public:

    Aircraft(int mType, int mConsumption, int mTankCapacity, int speed, int altitude);

    ~Aircraft();

    void print();

    void display(sf::RenderWindow &window, long double rotation, bool debug);

    void setPos(Pair pos);

    Pair getPos();

    void setSpeed(int speed);

    int getSpeed();

    int getSpeed(const Cell *cell);

    int getType ();

    int getTankCapacity();

    int getConsumption();

    int getAltitude();

    void setAltitude(int newAltitude);

    void doubleConsumption();

    int getFuite();

    void setFuite(bool fuite);

    void crash();

    bool isCrash();
};

#endif
