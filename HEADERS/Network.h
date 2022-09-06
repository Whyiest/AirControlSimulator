#ifndef CONTROLEAERIEN_GROUPE_2_G_NETWORK_H
#define CONTROLEAERIEN_GROUPE_2_G_NETWORK_H

#include "Libraries.h"
#include "util.h"
#include "Cell.h"
#include "Airport.h"
#include "Aircraft.h"
#include "FlightsPlanning.h"
#include "Disruption.h"

class Network {

private:

    std::vector<std::vector<Cell*>> m_cells;

    std::vector<Airport*> m_airportList;

    std::vector<Aircraft*> m_aircraftList;

    std::vector<FlightsPlanning*> m_flightPlanningList;

    std::vector<Disruption*> m_disruptionList;



public:

    Network();

    ~Network();

    void update(sf::RenderWindow &window, int ut);

    void loadData(const std::string& dataPath);

    void print();

    void initiateFlightPlanning ();

    FlightsPlanning* newFlightPlannings(Aircraft *withThisAirCraft);

    void defineCellTypes();

    //std::vector<Cell*> Astar(Pair src, Pair dest);

    void display(bool debug, sf::RenderWindow &window, Pair mouseGridPos);

    void changeAltitude();

    void appelerDijkstra(sf::RenderWindow &window);

    std::vector<Airport*> getAirportList();

    void addAircraft(Aircraft *newAirCraft);

    std::vector<FlightsPlanning *> getFlightsPlanningList();

    void addFlightPlanning(FlightsPlanning* newFP);
};

#endif
