#ifndef CONTROLEAERIEN_GROUPE_2_G_CELL_H
#define CONTROLEAERIEN_GROUPE_2_G_CELL_H

#include "Libraries.h"

class Cell {

private:

    Pair m_pos, m_parent;

    double m_Fcost, m_Gcost;

    int m_type; /// 1 == zone libre; 2 == zone en turbulence; 3 == zone autour d'un aéorport

    std::vector<Cell*> m_neighbours;

public:

    Cell(Pair pos);

    ~Cell();

    void addNeighbours(const std::vector<std::vector<Cell*>> &cells, bool addDiagonals);

    void resetAstarValues();

    void display(sf::RectangleShape &rect, sf::RenderWindow &window, bool debug);

    const Pair &getPos() const;

    Pair getParent() const;

    void setParent(Pair parent);

    double getFCost() const;

    void setFCost(double mFCost);

    double getGCost() const;

    void setGCost(double mGCost);

    int getType() const;

    void setType(int type);

    const std::vector<Cell *> &getNeighbours() const;

    void print();

};

#endif
