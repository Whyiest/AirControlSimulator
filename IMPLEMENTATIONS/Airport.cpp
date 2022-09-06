#include "../HEADERS/Airport.h"

///Constructeur
Airport::Airport(std::string Name, int ListID, Pair Pos, int NbTracks, int NbParkingSlots, int GroundWaitingTime,
                 int TrackAccessDelay, int AntiCollisionDelay, int LandingTime, int LoopWaitingTime) :
        m_name(std::move(Name)), m_listID(ListID), m_pos(std::move(Pos)), m_nbTracks(NbTracks), m_nbParkingSlots(NbParkingSlots),
        m_groundWaitingTime(GroundWaitingTime), m_trackAccessDelay(TrackAccessDelay),
        m_antiCollisionDelay(AntiCollisionDelay), m_landingTime(LandingTime), m_loopWaitingTime(LoopWaitingTime) {
    m_lastDepartureTime = 0;
}

///Destructeur
Airport::~Airport() = default;

///Ajoute un aéroport à a liste des voisins
void Airport::addNeighbour(Airport *airport, int distance) {
    if (airport == nullptr) std::cerr << "Warning (addNeighbour) : passing nullptr" << std::endl;
    if (distance < 0) std::cerr << "Warning (addNeighbour) : passing negative distance" << std::endl;
    m_neighbours.emplace_back(airport, distance);
}

///renvoi la distance entre cet aéroport et celui recherché dans la liste des voisins
int Airport::getDistanceTo(Airport *airport) {
    for (auto &neighbour : m_neighbours) {
        if (neighbour.first->getName() == airport->getName()) return neighbour.second;
    }
    return -1;
}

///Ecrit les infos dans la console
void Airport::print() {
    std::cout << "Nom de l'aeroport : " << m_name << std::endl
              << "Localisation x : " << m_pos.first << std::endl
              << "Localisation y : " << m_pos.second << std::endl
              << "Nombre de pistes : " << m_nbTracks << std::endl
              << "Nombre de places au sol : " << m_nbParkingSlots << std::endl
              << "Delai d'attente au sol : " << m_groundWaitingTime << std::endl
              << "Temps d'accès aux pistes : " << m_trackAccessDelay << std::endl
              << "Delai anticollision : " << m_antiCollisionDelay << std::endl
              << "Temps d'atterrissage : " << m_landingTime << std::endl
              << "Duree de la boucle d'attente en vol : " << m_loopWaitingTime << std::endl
              << std::endl;
}

///Ecrit les voisins et leur distance dans la console
void Airport::printNeighbours() {
    std::cout << "[Skyroad from " << m_name << "]" << std::endl;

    if (m_neighbours.empty()) {
        std::cout << "No neighbours" << std::endl;
        return;
    }

    for (auto &neighbour : m_neighbours) {
        std::cout << m_name << " --> " << neighbour.first->getName()
                  << " | Distance : " << neighbour.second << "km" << std::endl;
    }

    std::cout << std::endl;
}

///affiche les informations dans la fenetre dynamiquement
void Airport::display(sf::RectangleShape &rect, sf::RenderWindow &window, bool debug, Pair mouseGridPos) {
    if (inbound(mouseGridPos.first, mouseGridPos.second,
                m_pos.first - 3, m_pos.second - 3,
                m_pos.first + 3, m_pos.second + 3)) {

        std::vector<int> offsets = {-1, 0, 1};
        int x = map(m_pos.first, 0, GRID_WIDTH, 0, 3);
        int y = map(m_pos.second, 0, GRID_HEIGHT, 0, 3);
        int width = CELL_SIZE * 20;
        int height = CELL_SIZE * 10;
        int posX = x * GRID_WIDTH / 3 * CELL_SIZE + GRID_WIDTH / 6 * CELL_SIZE - width / 2 + offsets[x] * width / 2;
        int posY = y * GRID_HEIGHT / 3 * CELL_SIZE + GRID_HEIGHT / 6 * CELL_SIZE - height / 2 + offsets[y] * height / 2;
        drawLine(posX + width / 2, posY + height / 2,
                 m_pos.first * CELL_SIZE + CELL_SIZE / 2, m_pos.second * CELL_SIZE + CELL_SIZE / 2, 2,
                 sf::Color::Red, window);

        rect.setSize(sf::Vector2f(width, height));
        rect.setFillColor({100, 100, 100, 255});
        rect.setOutlineColor(sf::Color::Red);
        rect.setOutlineThickness(1);
        rect.setPosition(posX, posY);
        window.draw(rect);

        ///Affiche les routes pour chaque aéroport
        rect.setSize(sf::Vector2f(300, 140));
        rect.setFillColor({169, 169, 169, 255});
        rect.setPosition(900, 120);
        window.draw(rect);

        backgroundTextureRoad.loadFromImage(newImage("../DATA/road.png", 40, 120));
        backgroundSpriteRoad.setTexture(backgroundTextureRoad);

        for (int i = 0; i < m_nbTracks; ++i) {
            backgroundSpriteRoad.setPosition(i * 50 + 920, 130);
            window.draw(backgroundSpriteRoad);
        }

        ///Affiche les avions sur les pistes des aeroports
        backgroundTextureAircraft.loadFromImage(newImage("../DATA/aircraft.png", 50, 65));
        backgroundSpriteAircraft.setTexture(backgroundTextureAircraft);
        backgroundSpriteAircraft.setPosition(915, 150);
        window.draw(backgroundSpriteAircraft);

        displayText(posX + CELL_SIZE, posY, CELL_SIZE * 1.5, m_name, window);
        displayText(posX + CELL_SIZE, posY + CELL_SIZE * 2.5, CELL_SIZE,
                    "Pistes : " + std::to_string(m_nbTracks), window);
        displayText(posX + CELL_SIZE, posY + CELL_SIZE * 3.5, CELL_SIZE,
                    "Places au sol : " + std::to_string(m_nbParkingSlots), window);
        displayText(posX + CELL_SIZE, posY + CELL_SIZE * 4.5, CELL_SIZE,
                    "Attente au sol : " + std::to_string(m_groundWaitingTime) + " UT", window);
        displayText(posX + CELL_SIZE, posY + CELL_SIZE * 5.5, CELL_SIZE,
                    "Acces aux pistes : " + std::to_string(m_nbTracks) + " UT", window);
        displayText(posX + CELL_SIZE, posY + CELL_SIZE * 6.5, CELL_SIZE,
                    "Duree atter./decol. : " + std::to_string(m_trackAccessDelay) + " UT", window);
        displayText(posX + CELL_SIZE, posY + CELL_SIZE * 7.5, CELL_SIZE,
                    "Anti-collision : " + std::to_string(m_antiCollisionDelay) + " UT", window);
        displayText(posX + CELL_SIZE, posY + CELL_SIZE * 8.5, CELL_SIZE,
                    "Boucle d'attente : " + std::to_string(m_loopWaitingTime) + " UT", window);
    }

    rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    rect.setOutlineThickness(0);
    rect.setPosition(m_pos.first * CELL_SIZE, m_pos.second * CELL_SIZE);
    rect.setFillColor({255, 0, 0, 155});
    if (debug) window.draw(rect); ///dessine le centre de la zone d'aéroport

    for (auto &neighbour: m_neighbours) { ///dessine les lignes et leurs distances entre les aéroports
        drawLine(m_pos.first * CELL_SIZE + CELL_SIZE / 2,
                 m_pos.second * CELL_SIZE + CELL_SIZE / 2,
                 neighbour.first->getPos().first * CELL_SIZE + CELL_SIZE / 2,
                 neighbour.first->getPos().second * CELL_SIZE + CELL_SIZE / 2,
                 2,
                 sf::Color::Green,
                 window);
        displayText((m_pos.first + neighbour.first->getPos().first) / 2 * CELL_SIZE,
                    (m_pos.second + neighbour.first->getPos().second) / 2 * CELL_SIZE - 10,
                    1.2 * CELL_SIZE, std::to_string(neighbour.second), window);
    }
}

///GETTERS
std::string Airport::getName() {
    return m_name;
}

Pair Airport::getPos() {
    return m_pos;
}

std::vector<std::pair<Airport *, int>> Airport::getNeighbours() {
    return m_neighbours;
}

int Airport::getLoopWaitingTime() {
    return m_loopWaitingTime;
}

int Airport::getListID() {
    return m_listID;
}

void Airport::setListID(int id) {
    m_listID = id;
}

int Airport::getAntiCollisionDelay() const {
    return m_antiCollisionDelay;
}

void Airport::setAntiCollisionDelay(int mAntiCollisionDelay) {
    m_antiCollisionDelay = mAntiCollisionDelay;
}

int Airport::getLastDepartureTime() const {
    return m_lastDepartureTime;
}

void Airport::setLastDepartureTime(int mLastDepartureTime) {
    m_lastDepartureTime = mLastDepartureTime;
}
