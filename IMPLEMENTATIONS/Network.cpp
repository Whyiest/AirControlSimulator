#include "../HEADERS/Network.h"

///Constructeur
Network::Network() {
    m_cells.resize(GRID_WIDTH);
    for (int x = 0; x < m_cells.size(); ++x) {
        m_cells[x].resize(GRID_HEIGHT);
        for (int y = 0; y < m_cells[x].size(); ++y) {
            m_cells[x][y] = new Cell({x, y});
        }
    }
    for (auto &cell: m_cells) for (auto &c: cell) c->addNeighbours(m_cells, true);

    int centrage = 20;
    for (int i = 0; i < randomInt(10, 15); ++i) {
        m_disruptionList.push_back(new Disruption({randomInt(centrage, GRID_WIDTH - centrage),
                                                   randomInt(centrage, GRID_HEIGHT - centrage)},
                                                  randomInt(1, CELL_SIZE / 2)));
    }
}

///Destructeur
Network::~Network() {
    for (auto &cell: m_cells) for (auto &c: cell) delete c;
    for (auto &airport: m_airportList) delete airport;
    for (auto &aircraft: m_aircraftList) delete aircraft;
    for (auto &planning: m_flightPlanningList) delete planning;
    for (auto &disruption: m_disruptionList) delete disruption;
}

///Boucle d'actualisation
void Network::update(sf::RenderWindow &window, int ut) {
    for (auto &disruption: m_disruptionList) disruption->update();
    for (auto &planning: m_flightPlanningList) {
        if (!planning->hasStarted() && !planning->hasArrived() && randomInt(0, 100) < 10 &&
            ut - planning->getDeparture()->getLastDepartureTime() >=
            planning->getDeparture()->getAntiCollisionDelay()) {
            planning->setHasStarted(true);
            planning->setDepartureTime(ut);
            planning->getDeparture()->setLastDepartureTime(ut);

            std::cout << "Un avion est parti de "
                      << planning->getDeparture()->getName()
                      << " a "
                      << conversionUTHeure(ut)
                      << std::endl;
        }
        planning->update(m_cells, ut);
        changeAltitude();


        for (int i = 0; i < m_aircraftList.size() - 1; ++i) {
            if (randomInt(0, 10000) == 2300 && m_aircraftList[i]->getFuite() != true) {
                m_aircraftList[i]->doubleConsumption();
            }
        }
        appelerDijkstra(window);
    }
}

///Charge les données du ficher texte

void Network::loadData(const std::string &dataPath) {

    std::string name;
    Pair pos;
    int nbTracks;
    int nbParkingSlots;
    int groundWaitingTime;
    int trackAccessDelay;
    int antiCollisionDelay;
    int landingTime;
    int loopWaitingTime;

    int distance;
    int size;
    int ordre;
    int nbAircrafts;

    int type; ///1 : short    2 : medium  3 : long
    int consumption;
    int tankCapacity;
    float speed;
    int altitude = 10000;

    std::string airport1;
    std::string airport2;

    /// VERIF FICHIER :

    std::ifstream ifs{dataPath};
    if (!ifs) throw std::runtime_error("Unable to open file : " + dataPath);

    ifs >> ordre; // Ordre de notre graphe
    if (ifs.fail()) throw std::runtime_error("Unable to get ordre");

    ifs >> size; // La taille de notre graphe, soit le nommbre de routes aérienne
    if (ifs.fail()) throw std::runtime_error("Unable to get size");

    ifs >> nbAircrafts; // Le nombre d'avions
    if (ifs.fail()) throw std::runtime_error("Unable to get nbAircrafts");

    /// UTILISATION FICHIER :

    for (int i = 0; i < ordre; ++i) { // Pour chaque description d'aéroport, créeer l'aéroport lié

        ifs >> name;
        if (ifs.fail()) throw std::runtime_error("Unable to get name");
        ifs >> pos.first;
        if (ifs.fail()) throw std::runtime_error("Unable to get pos.first");
        ifs >> pos.second;
        if (ifs.fail()) throw std::runtime_error("Unable to get pos.second");
        ifs >> nbTracks;
        if (ifs.fail()) throw std::runtime_error("Unable to get nbTracks");
        ifs >> nbParkingSlots;
        if (ifs.fail()) throw std::runtime_error("Unable to get nbParkingSlots");
        ifs >> groundWaitingTime;
        if (ifs.fail()) throw std::runtime_error("Unable to get groundWaitingTime");
        ifs >> trackAccessDelay;
        if (ifs.fail()) throw std::runtime_error("Unable to get trackAccessDelay");
        ifs >> antiCollisionDelay;
        if (ifs.fail()) throw std::runtime_error("Unable to get antiCollisionDelay");
        ifs >> landingTime;
        if (ifs.fail()) throw std::runtime_error("Unable to get landingTime");
        ifs >> loopWaitingTime;
        if (ifs.fail()) throw std::runtime_error("Unable to get loopWaitingTime");

        pos.first = map(pos.first, 0, 144, 0, GRID_WIDTH);
        pos.second = map(pos.second, 0, 80, 0, GRID_HEIGHT);

        m_airportList.push_back(new Airport(name, i, pos, nbTracks, nbParkingSlots, groundWaitingTime, trackAccessDelay,
                                            antiCollisionDelay, landingTime, loopWaitingTime));
    }

    /// CREATION DES ARCS (VOIE AERIENNES)

    for (int h = 0; h < size; h++) {

        ifs >> airport1;
        if (ifs.fail()) throw std::runtime_error("Unable to get airport1");
        ifs >> airport2;
        if (ifs.fail()) throw std::runtime_error("Unable to get airport2");
        ifs >> distance;
        if (ifs.fail()) throw std::runtime_error("Unable to get distance");

        for (auto &i: m_airportList) {
            if (i->getName() == airport1) {
                for (auto &j: m_airportList) {
                    if (j->getName() == airport2) {
                        i->addNeighbour(j, distance);
                        j->addNeighbour(i, distance);
                    }
                }
            }
        }
    }

    for (int i = 0; i < nbAircrafts; ++i) {
        ifs >> type;
        if (ifs.fail()) throw std::runtime_error("Unable to get type");

        switch (type) {
            case 1:
                consumption = 80;
                tankCapacity = 7000;
                speed = 2;
                break;
            case 2:
                consumption = 100;
                tankCapacity = 10000;
                speed = 3;
                break;
            case 3:
                consumption = 120;
                tankCapacity = 15000;
                speed = 4;
                break;
            default:
                consumption = 0;
                tankCapacity = 0;
                speed = 0;
                break;
        }

        m_aircraftList.push_back(new Aircraft(type, consumption, tankCapacity, speed, altitude));
    }
}

void Network::print() {
    for (auto &i: m_airportList) {
        i->print();
        i->printNeighbours();
    }

    for (int i = 0; i < m_aircraftList.size() - 1; ++i) {
        std::cout << "aircraft : " << i << std::endl;
        m_aircraftList[i]->print();
    }
}

void Network::initiateFlightPlanning() {

    /// Initialise au début tous les flights planning (autant qu'il y a d'avions

    m_flightPlanningList.resize(m_aircraftList.size());

    for (int j = 0; j < m_aircraftList.size(); j++) {
        m_flightPlanningList[j] = newFlightPlannings(m_aircraftList[j]);
    }
}

FlightsPlanning *Network::newFlightPlannings(Aircraft *withThisAirCraft) {

    /// Créer un nouveau flight planning en initialisant les infos

    auto *newFP = new FlightsPlanning(withThisAirCraft);

    do {
        do {
            newFP->clearSteps();
            newFP->setDeparture(m_airportList[randomInt(0, m_airportList.size() - 1)]);
            newFP->setArrival(m_airportList[randomInt(0, m_airportList.size() - 1)]);
        } while (newFP->getDeparture() == newFP->getArrival());

        newFP->setStep(newFP->dijkstra(m_airportList));

    } while (newFP->getSteps()[newFP->getSteps().size() - 1] != newFP->getArrival());

    newFP->calculateDistance();
    newFP->autoFillOtherInfo();

    return newFP;
}

///Définit le type de cellule en fonction des aéroport et de la météo
void Network::defineCellTypes() {

    for (auto &cell: m_cells) for (auto &c: cell) c->setType(1);

    for (auto &disruption: m_disruptionList) {
        int power = disruption->getPower();
        for (int x = -power + disruption->getPos().first; x < power + disruption->getPos().first + 1; ++x) {
            for (int y = -power + disruption->getPos().second; y < power + disruption->getPos().second + 1; ++y) {
                if (inbound(x, y, 0, 0, GRID_WIDTH, GRID_HEIGHT)) m_cells[x][y]->setType(2);
            }
        }
    }

    for (auto &airport: m_airportList) {
        int time = airport->getLoopWaitingTime();
        for (int x = -time + airport->getPos().first; x < time + airport->getPos().first + 1; ++x) {
            for (int y = -time + airport->getPos().second; y < time + airport->getPos().second + 1; ++y) {
                if (inbound(x, y, 0, 0, GRID_WIDTH, GRID_HEIGHT)) m_cells[x][y]->setType(3);
            }
        }
    }
}

///Affiche la grille
void Network::display(bool debug, sf::RenderWindow &window, Pair mouseGridPos) {
    sf::RectangleShape rect;

    ///dessine les cases
    for (auto &cell: m_cells) for (auto &c: cell) c->display(rect, window, debug);

    ///dessine les perturbations
    for (auto &disruption: m_disruptionList) disruption->display(rect, window, debug);

    ///dessine les aéroports
    for (auto &airport: m_airportList) airport->display(rect, window, debug, mouseGridPos);

    ///dessine les avions et leur trajectoire
    for (auto &fp : m_flightPlanningList) fp->display(rect, window, debug, mouseGridPos);
}


///si deux avions sont sur la même trajectoire leur altitude change
void Network::changeAltitude() {
    for (int i = 0; i < m_aircraftList.size() - 1; i++) {

        for (auto & j : m_airportList) if (m_aircraftList[i]->getPos() == j->getPos()) return;

        if ((m_aircraftList[i]->getPos().first == m_aircraftList[i + 1]->getPos().first) &&
            (m_aircraftList[i]->getPos().second == m_aircraftList[i + 1]->getPos().second)) {
            std::cout << "Changement d'altitude des avions : " << std::endl;
            m_aircraftList[i]->setAltitude(8000);
            m_aircraftList[i + 1]->setAltitude(12000);
            std::cout << "Avion 1 : " << m_aircraftList[i]->getAltitude() << std::endl;
            std::cout << "Avion 2 : " << m_aircraftList[i + 1]->getAltitude() << std::endl;
        }
    }
}

void Network::appelerDijkstra(sf::RenderWindow &window) {

    std::vector<std::pair<Airport *, int>> result;
    int lowerDistance = std::numeric_limits<int>::max();
    int tempID = -1;

    Airport *newDirection;
    Airport *OldArrival;
    Airport *OldDeparture;

    for (auto &air : m_flightPlanningList) {

        if (air->getAircraft()->getFuite() == true) {

            result = air->dijkstraInFlight(m_airportList);

            for (int i = 0; i < result.size() - 1; ++i) {

                if (result[i].first != air->getSteps()[air->getCurrentStep()]) {
                    if (result[i].second < lowerDistance) {
                        tempID = i;
                        lowerDistance = result[i].second;
                    }
                }
            }

            newDirection = result[tempID].first;
            OldDeparture = air->getSteps()[air->getCurrentStep()];
            OldArrival = air->getSteps()[air->getCurrentStep() + 1];

            air->clearSteps();
            air->setCurrentStep(0);
            air->addStep(OldDeparture);
            air->addStep(newDirection);

            if (result[tempID].second > air->getAircraft()->getTankCapacity() || air->getSteps()[1] == OldArrival) {

                air->crash();
            }

        }

    }

}

/*///Algorithme A* qui trouve le plus chemin d'un graphe non pondéré (à utiliser sur la grille)
std::vector<Cell *> Network::Astar(Pair source, Pair destination) {

    std::vector<Cell *> path;

    if (source == destination) return path;

    if (!inbound(source.first, source.second, 0, 0, GRID_WIDTH, GRID_HEIGHT)) {
        std::cerr << "Error (Astar) : invalid source (" << source.first << ", " << source.second << ")" << std::endl;
        return path;
    }

    if (!inbound(destination.first, destination.second, 0, 0, GRID_WIDTH, GRID_HEIGHT)) {
        std::cerr << "Error (Astar) : invalid destination (" << destination.first << ", " << destination.second << ")"
                  << std::endl;
        return path;
    }

    std::vector<std::vector<bool>> closedList(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));
    for (auto &cell: m_cells) {
        for (auto &c: cell) {
            c->resetAstarValues();
        }
    }

    int x = source.first;
    int y = source.second;
    m_cells[x][y]->setFCost(0);
    m_cells[x][y]->setGCost(0);
    m_cells[x][y]->setParent({x, y});
    std::priority_queue<Tuple, std::vector<Tuple>, std::greater<>> openList;
    openList.emplace(0, x, y);

    while (!openList.empty()) {

        const Tuple &t = openList.top();
        x = std::get<1>(t);
        y = std::get<2>(t);
        openList.pop();
        closedList[x][y] = true;

        for (auto &neighbour: m_cells[x][y]->getNeighbours()) {

            if (neighbour->getPos() == destination) {

                m_cells[neighbour->getPos().first][neighbour->getPos().second]->setParent({x, y});
                int row = destination.first;
                int col = destination.second;
                Pair next = m_cells[row][col]->getParent();

                do {
                    path.push_back(m_cells[next.first][next.second]);
                    next = m_cells[row][col]->getParent();
                    row = next.first;
                    col = next.second;
                } while (m_cells[row][col]->getParent() != next);

                path.emplace_back(m_cells[row][col]);
                return path;

            } else if (!closedList[neighbour->getPos().first][neighbour->getPos().second] &&
                       m_cells[neighbour->getPos().first][neighbour->getPos().second]->getType() != 0) {

                double gNew, fNew;
                gNew = m_cells[x][y]->getGCost() + 1;
                fNew = gNew + sqrt(pow((neighbour->getPos().first - destination.first), 2) +
                                   pow((neighbour->getPos().second - destination.second), 2));

                if (m_cells[neighbour->getPos().first][neighbour->getPos().second]->getFCost() == -1 ||
                    m_cells[neighbour->getPos().first][neighbour->getPos().second]->getFCost() > fNew) {

                    openList.emplace(fNew, neighbour->getPos().first, neighbour->getPos().second);
                    m_cells[neighbour->getPos().first][neighbour->getPos().second]->setGCost(gNew);
                    m_cells[neighbour->getPos().first][neighbour->getPos().second]->setFCost(fNew);
                    m_cells[neighbour->getPos().first][neighbour->getPos().second]->setParent({x, y});
                }
            }
        }
    }

    return path;
}*/

void Network::addAircraft (Aircraft* newAirCraft) {
    m_aircraftList.push_back(newAirCraft);
}

std::vector<FlightsPlanning*> Network::getFlightsPlanningList() {
    return m_flightPlanningList;
}

void Network::addFlightPlanning(FlightsPlanning* newFP) {
   m_flightPlanningList.push_back(newFP);
}