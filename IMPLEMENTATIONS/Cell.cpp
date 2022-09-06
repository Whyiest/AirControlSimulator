#include "../HEADERS/Cell.h"

///Constructeur
Cell::Cell(Pair pos) : m_pos(pos){
    m_parent = {-1, -1};
    m_Fcost = -1;
    m_Gcost = -1;
    m_type = 1;
}

///Destrcteur
Cell::~Cell() = default;

///Créer la liste des voisins de la grille (droite gauche bas haut)
void Cell::addNeighbours(const std::vector<std::vector<Cell *>> &cells, bool addDiagonals) {
    if (m_pos.first < GRID_WIDTH - 1) m_neighbours.push_back(cells[m_pos.first + 1][m_pos.second]);
    if (m_pos.first > 0) m_neighbours.push_back(cells[m_pos.first - 1][m_pos.second]);
    if (m_pos.second < GRID_HEIGHT - 1) m_neighbours.push_back(cells[m_pos.first][m_pos.second + 1]);
    if (m_pos.second > 0) m_neighbours.push_back(cells[m_pos.first][m_pos.second - 1]);
    if (!addDiagonals) return;
    if (m_pos.first < GRID_WIDTH - 1 && m_pos.second < GRID_HEIGHT - 1) m_neighbours.push_back(cells[m_pos.first + 1][m_pos.second + 1]);
    if (m_pos.first < GRID_WIDTH - 1 && m_pos.second > 0) m_neighbours.push_back(cells[m_pos.first + 1][m_pos.second - 1]);
    if (m_pos.first > 0 &&  m_pos.second < GRID_HEIGHT - 1) m_neighbours.push_back(cells[m_pos.first - 1][m_pos.second + 1]);
    if (m_pos.first > 0 &&  m_pos.second > 0) m_neighbours.push_back(cells[m_pos.first - 1][m_pos.second - 1]);
}

///Réinitialise les valeurs utilisées par A*
void Cell::resetAstarValues() {
    m_parent = {-1, -1};
    m_Fcost = -1;
    m_Gcost = -1;
}

void Cell::display(sf::RectangleShape &rect, sf::RenderWindow &window, bool debug) {
    rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    rect.setOutlineThickness(CELL_SIZE >= 10);
    rect.setOutlineColor({100, 100, 100, 100});

    switch (m_type) {
        case 1:
            rect.setFillColor({50, 50, 50, 100});
            break;
        case 2:
            rect.setFillColor({255, 0, 255, 155});
            break;
        case 3:
            rect.setFillColor({255, 0, 0, 155});
            break;
    }

    rect.setPosition(m_pos.first * CELL_SIZE, m_pos.second * CELL_SIZE);
    window.draw(rect);
}

///Ecrit les infos dans la console
void Cell::print() {
    std::cout << "m_pos.first: " << m_pos.first << std::endl
              << "m_pos.second: " << m_pos.second << std::endl
              << "m_parent.first: " << m_parent.first << std::endl
              << "m_parent.second: " << m_parent.second << std::endl
              << "m_Fcost: " << m_Fcost << std::endl
              << "m_Gcost: " << m_Gcost << std::endl
              << "m_type: " << m_type << std::endl;
}

///GETTERS SETTERS

const Pair &Cell::getPos() const {
    return m_pos;
}

Pair Cell::getParent() const {
    return m_parent;
}

void Cell::setParent(Pair parent) {
    m_parent = parent;
}

double Cell::getFCost() const {
    return m_Fcost;
}

void Cell::setFCost(double mFCost) {
    m_Fcost = mFCost;
}

double Cell::getGCost() const {
    return m_Gcost;
}

void Cell::setGCost(double mGCost) {
    m_Gcost = mGCost;
}

int Cell::getType() const {
    return m_type;
}

void Cell::setType(int type) {
    m_type = type;
}

const std::vector<Cell *> &Cell::getNeighbours() const {
    return m_neighbours;
}
