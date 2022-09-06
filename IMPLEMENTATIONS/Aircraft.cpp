#include "../HEADERS/Aircraft.h"

///Constructeur
Aircraft::Aircraft(int mType, int mConsumption, int mTankCapacity, int speed, int altitude) :
        m_type(mType), m_consumption(mConsumption), m_tankCapacity(mTankCapacity), m_speed(speed), m_altitude(altitude),
        m_fuite(false), m_crash(false) {
    m_pos = {-1, -1};

    switch (m_type) {
        case 1:
            avionTexture.loadFromImage(newImage("../DATA/avionPetit.png", 3 * CELL_SIZE, 3 * CELL_SIZE));
            avionSprite.setTexture(avionTexture);
            avionSprite.setOrigin(3 * CELL_SIZE / 2, 3 * CELL_SIZE / 2);
            break;
        case 2:
            avionTexture.loadFromImage(newImage("../DATA/avionMoyen.png", 5 * CELL_SIZE, 5 * CELL_SIZE));
            avionSprite.setTexture(avionTexture);
            avionSprite.setOrigin(5 * CELL_SIZE / 2, 5 * CELL_SIZE / 2);
            break;
        case 3:
            avionTexture.loadFromImage(newImage("../DATA/avionGrand.png", 7 * CELL_SIZE, 7 * CELL_SIZE));
            avionSprite.setTexture(avionTexture);
            avionSprite.setOrigin(7 * CELL_SIZE / 2, 7 * CELL_SIZE / 2);
            break;
    }
}

///Destrcteur
Aircraft::~Aircraft() = default;

///Ecrit les infos dans la console
void Aircraft::print() {

    switch (m_type) {
        case 1:
            std::cout << "Type court " << std::endl;
            break;
        case 2:
            std::cout << "Type moyen " << std::endl;
            break;
        case 3:
            std::cout << "Type long " << std::endl;
            break;
    }

    std::cout << "Consommation : " << m_consumption << std::endl;
    std::cout << "Capacite du carburant : " << m_tankCapacity << std::endl;

    std::cout << std::endl;
}

void Aircraft::display(sf::RenderWindow &window, long double rotation, bool debug) {

    if (debug) {

        sf::CircleShape circ;
        switch (m_type) {
            case 1:
                circ.setFillColor(sf::Color::Yellow);
                break;
            case 2:
                circ.setFillColor(sf::Color::Red);
                break;
            case 3:
                circ.setFillColor(sf::Color::Green);
                break;
        }
        circ.setRadius(CELL_SIZE / 2);
        circ.setPosition(m_pos.first * CELL_SIZE, m_pos.second * CELL_SIZE);
        window.draw(circ);

    }
    if (debug == false && m_crash == false) {

        avionSprite.setPosition(m_pos.first * CELL_SIZE, m_pos.second * CELL_SIZE);
        avionSprite.setRotation(rotation);

        window.draw(avionSprite);
    }
    if (debug == false && m_crash == true) {

        sf::Texture crashPlaneTexture;
        sf::Sprite crashPlaneSprite;
        crashPlaneTexture.loadFromImage(newImage("../DATA/explosion.png", 3 * CELL_SIZE, 3 * CELL_SIZE));
        crashPlaneSprite.setPosition(m_pos.first * CELL_SIZE, m_pos.second * CELL_SIZE);
        crashPlaneSprite.setTexture(crashPlaneTexture);
        window.draw(crashPlaneSprite);

    }
}

void Aircraft::setPos(Pair pos) {
    m_pos = pos;
}

Pair Aircraft::getPos() {
    return m_pos;
}


void Aircraft::setSpeed(int speed) {
    m_speed = speed;
}

int Aircraft::getSpeed() {
    return m_speed;
}

///Définition de la vitesse de la fonction en fonction des types et de la zone dans laquelle l'avion circule
int Aircraft::getSpeed(const Cell *cell) {
    switch (cell->getType()) {
        case 1:
            return std::round(m_speed); ///vitesse normale
        case 2:
            return std::round(m_speed * 0.5); ///mauvaise météo -50% de la vitesse
        case 3:
            return 1; ///autour d'un aéroport vitesse 1
    }
    return std::round(m_speed);
}

int Aircraft::getType() {
    return m_type;
}

int Aircraft::getTankCapacity() {
    return m_tankCapacity;
}

int Aircraft::getConsumption() {
    return m_consumption;
}

void Aircraft::doubleConsumption() {
    m_consumption *= 2;
    m_fuite = true;
}

int Aircraft::getAltitude() {
    return m_altitude;
}

void Aircraft::setAltitude(int newAltitude) {
    m_altitude = newAltitude;
}

int Aircraft::getFuite() {
    return m_fuite;
}

void Aircraft::setFuite(bool fuite) {
    m_fuite = fuite;
}

void Aircraft::crash() {
    m_crash = true;
}

bool Aircraft::isCrash() {
    return  m_crash;
}