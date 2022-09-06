#include "../HEADERS/FlightsPlanning.h"

///******************************************************************** CONSTRUCTEUR ************************************************************///
FlightsPlanning::FlightsPlanning(Aircraft *aircraft) {
    m_hasStarted = false;
    m_isDirect = false;
    m_distance = 0;
    m_nbSteps = 0;
    m_departure = nullptr;
    m_arrival = nullptr;
    m_aircraft = aircraft;
}

FlightsPlanning::~FlightsPlanning() = default;

void FlightsPlanning::update(std::vector<std::vector<Cell *>> &cells, int ut) {
    if (m_hasStarted && !m_hasArrived) {
        Astar(cells, ut);
        Pair a_pos = m_aircraft->getPos();
        //std::cout << m_aircraft->getSpeed(cells[a_pos.first][a_pos.second]) << std::endl;
        for (int i = 0; i < m_aircraft->getSpeed(cells[a_pos.first][a_pos.second]); ++i) {
            m_aircraft->setPos(m_path[i]->getPos());

            if (a_pos == m_steps[m_currentStep + 1]->getPos()) {
                if (m_currentStep + 1 < m_nbSteps - 1) m_currentStep++;

                if (randomInt(0, 100) < 10) {
                    m_aircraft->doubleConsumption();
                    //FAIRE EN SORTE NETWORK.CPP APPELLE DIJKSTRA POUR TOUT LES FLIGHTPLANNING EN FUITE
                }
            }

            if (a_pos == m_arrival->getPos()) {
                m_hasArrived = true;
                std::cout << "Un avion est arrive a "
                          << m_departure->getName()
                          << " a "
                          << conversionUTHeure(ut)
                          << std::endl;
            }
        }
    }
}

std::vector<Airport *> FlightsPlanning::dijkstra(const std::vector<Airport *> &airportList) {

    // Valeur retournée en cas d'erreurs :

    std::vector<Airport *> error(1, nullptr);

    // Vérification de sécurité :

    //std::cout << "[Dijkstra] Recherche d'un chemin entre " << m_departure->getName() << " et " << m_arrival->getName() << std::endl;
    //std::cout << "Type d'avion : " << m_aircraft->getType() << " (reservoir : " << m_aircraft->getTankCapacity() << "km)" << std::endl;

    if (m_departure == nullptr) {
        std::cerr << "Warning (dijkstra) : m_departure = nullptr" << std::endl;
        return error;
    }
    if (m_arrival == nullptr) {
        std::cerr << "Warning (dijkstra) : m_arrival = nullptr" << std::endl;
        return error;
    }
    if (m_departure == m_arrival) {
        std::cerr << "Echec (dijkstra) : m_departure = m_arrival" << std::endl;
        return error;
    }

    // Adaptation du réservoir au type d'avion pour le calcul du trajet :
    int tankCapacity = m_aircraft->getTankCapacity();

    // Initialisaiton des vecteurs
    std::vector<bool> visited(airportList.size(), false); // Couleur
    std::vector<Airport *> predecesor(airportList.size(), nullptr);
    std::vector<int> distances(airportList.size(), std::numeric_limits<int>::max());


    // Définissons notre aéroport de départ :

    int idCurrentAirport = -1;
    distances[m_departure->getListID()] = 0;

    // Queue de priorité auto rangée : elle met en avant les plus courtes distances :

    std::priority_queue<Pair, std::vector<Pair>, std::less<>> queue;

    // On y ajoute l'aéroport de départ :

    queue.push({0, m_departure->getListID()});

    // Pour chaque aéroport en haut de la pile :

    while (!queue.empty()) {

        idCurrentAirport = queue.top().second;
        queue.pop();


        visited[idCurrentAirport] = true;

        // On travail sur les successeurs :

        for (auto &neighbour: airportList[idCurrentAirport]->getNeighbours()) {

            if (visited[neighbour.first->getListID()] == false) {
                // S'il n'a pas déjà été marqué

                if (distances[idCurrentAirport] + neighbour.second < distances[neighbour.first->getListID()]) {
                    // Si la distance depuis S0 jusqu'à l'aéroport actuel + la distance entre le voisins et l'aéroport actuel
                    // est inférieure à la distance déjà enregistrer depuis S0 pour ce voisin, alors :

                    if (neighbour.second <= tankCapacity) {

                        distances[neighbour.first->getListID()] = distances[idCurrentAirport] + neighbour.second;
                        predecesor[neighbour.first->getListID()] = airportList[idCurrentAirport];
                    } else {
                        //std::cout << "Dijkstra : carburant insuffisant entre les deux " << std::endl;
                    }
                }
                // Puis on le met dans la file :
                queue.push({distances[neighbour.first->getListID()], neighbour.first->getListID()});

            }
        }
    }

    /// ------------------------ FIN DU TRI DIJKSTRA :

    // Maintenant que Dijkstra est terminé, nous récupérons le plus cours chemin entre le départ et l'arrivé :

    std::vector<Airport *> toReturn;
    int temp;

    toReturn.push_back(m_arrival); // On commence par la fin

    while (toReturn[toReturn.size() - 1]->getName() != m_departure->getName()) { // Tant qu'on est pas revenu au début

        temp = toReturn[toReturn.size() -
                        1]->getListID(); // Le dernier aéroport à avoir été ajouté à la liste (max - 1 car on commence à 0)

        if (predecesor[temp] == nullptr) {
            //std::cout << "Echec de Dijkstra : impossible de relier ces deux aeroport avec ce type d'avion" << std::endl << std::endl;
            return error;
        } else {
            toReturn.push_back(predecesor[temp]); // On push son prédécesseur
        }
    }

    // Le vecteur est à l'envers, mettons le à l'endroit :

    std::reverse(toReturn.begin(), toReturn.end());

    /// Partie affichage :

    // Affichage de toutes les distances issues de Dijkstra

    //std::cout << "Distances global : " << std::endl;
/*
    if (toReturn.size() > 1) {
        for (int i = 0; i < distances.size() - 1; ++i) {

            std::cout << m_departure->getName() << " -> ";

            for (auto air: airportList) {

                if (air->getListID() == i) {

                    if (distances[i] == std::numeric_limits<int>::max()) {
                        std::cout << air->getName() << " : impossible" << std::endl;
                    } else {
                        std::cout << air->getName() << " : " << distances[i] << std::endl;

                    }
                }
            }
        }
    }*/

    // Affichage final en console :

    std::cout << "Le chemin le plus cours pour aller de " << m_departure->getName() << " a " << m_arrival->getName()
              << " est :" << std::endl;

    if (toReturn.size() > 1) {
        displayPath(toReturn);
        m_currentStep = 0;
        m_aircraft->setPos(m_departure->getPos());
    } else {
        m_currentStep = -1;
        return error;
    }

    // Retourne la liste des aéroports :
    return toReturn;
}

void FlightsPlanning::Astar(std::vector<std::vector<Cell *>> &cells, int ut) {
    m_path.clear();

    Pair source = m_steps[m_currentStep + 1]->getPos();
    Pair destination = m_aircraft->getPos();

    if (source == destination) {
        //std::cerr << "Error (Astar) : invalid source and destination are the same location" << std::endl;
        return;
    }

    if (!inbound(source.first, source.second, 0, 0, GRID_WIDTH, GRID_HEIGHT)) {
        std::cerr << "Error (Astar) : invalid source (" << source.first << ", " << source.second << ")" << std::endl;
        return;
    }

    if (!inbound(destination.first, destination.second, 0, 0, GRID_WIDTH, GRID_HEIGHT)) {
        std::cerr << "Error (Astar) : invalid destination (" << destination.first << ", " << destination.second << ")"
                  << std::endl;
        return;
    }

    std::vector<std::vector<bool>> closedList(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));
    for (auto &cell: cells) {
        for (auto &c: cell) {
            c->resetAstarValues();
        }
    }

    int x = source.first;
    int y = source.second;
    cells[x][y]->setFCost(0);
    cells[x][y]->setGCost(0);
    cells[x][y]->setParent({x, y});
    std::priority_queue<Tuple, std::vector<Tuple>, std::greater<>> openList;
    openList.emplace(0, x, y);

    while (!openList.empty()) {

        const Tuple &t = openList.top();
        x = std::get<1>(t);
        y = std::get<2>(t);
        openList.pop();
        closedList[x][y] = true;

        for (auto &neighbour: cells[x][y]->getNeighbours()) {

            if (neighbour->getPos() == destination) {

                cells[neighbour->getPos().first][neighbour->getPos().second]->setParent({x, y});
                //m_path.push_back(cells[neighbour->getPos().first][neighbour->getPos().second]);
                int row = destination.first;
                int col = destination.second;
                Pair next = cells[row][col]->getParent();

                do {
                    m_path.push_back(cells[next.first][next.second]);
                    next = cells[row][col]->getParent();
                    row = next.first;
                    col = next.second;
                } while (cells[row][col]->getParent() != next);

                m_path.emplace_back(cells[row][col]);
                return;

            } else if (!closedList[neighbour->getPos().first][neighbour->getPos().second] &&
                       cells[neighbour->getPos().first][neighbour->getPos().second]->getType() != 0) {

                double gNew, fNew;
                gNew = cells[x][y]->getGCost() + 1;
                fNew = gNew + sqrt(pow((neighbour->getPos().first - destination.first), 2) +
                                   pow((neighbour->getPos().second - destination.second), 2));

                if (cells[neighbour->getPos().first][neighbour->getPos().second]->getFCost() == -1 ||
                    cells[neighbour->getPos().first][neighbour->getPos().second]->getFCost() > fNew) {

                    openList.emplace(fNew, neighbour->getPos().first, neighbour->getPos().second);
                    cells[neighbour->getPos().first][neighbour->getPos().second]->setGCost(gNew);
                    cells[neighbour->getPos().first][neighbour->getPos().second]->setFCost(fNew);
                    cells[neighbour->getPos().first][neighbour->getPos().second]->setParent({x, y});
                }
            }
        }
    }
}

void FlightsPlanning::display(sf::RectangleShape &rect, sf::RenderWindow &window, bool debug, Pair mouseGridPos) {
    if (m_hasStarted && !m_hasArrived) {
        if (inbound(mouseGridPos.first, mouseGridPos.second,
                    m_aircraft->getPos().first - 3, m_aircraft->getPos().second - 3,
                    m_aircraft->getPos().first + 3, m_aircraft->getPos().second + 3)) {


            for (int i = 0; i < m_steps.size() - 1; ++i) {
                drawLine(m_steps[i]->getPos().first * CELL_SIZE + CELL_SIZE / 2,
                         m_steps[i]->getPos().second * CELL_SIZE + CELL_SIZE / 2,
                         m_steps[i + 1]->getPos().first * CELL_SIZE + CELL_SIZE / 2,
                         m_steps[i + 1]->getPos().second * CELL_SIZE + CELL_SIZE / 2,
                         4, sf::Color::Cyan, window);
            }

            rect.setOutlineThickness(1);
            rect.setOutlineColor(sf::Color::Black);
            rect.setFillColor(sf::Color::Cyan);
            for (auto &cell: m_path) {
                rect.setPosition(cell->getPos().first * CELL_SIZE, cell->getPos().second * CELL_SIZE);
                window.draw(rect);
            }

            displayInfosAircraft(window);
        }

        m_aircraft->display(window, getRotation(), debug);
    }
}

void FlightsPlanning::displayInfosAircraft(sf::RenderWindow &window) {
    std::vector<int> offsets = {-1, 0, 1};
    int x = m_aircraft->getPos().first * CELL_SIZE - CELL_SIZE * 10;
    int y = m_aircraft->getPos().second * CELL_SIZE + 30;
    int width = CELL_SIZE * 20;
    int height = CELL_SIZE * 10;
    int xvol = 50;
    int yvol = 500;

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(width, height));
    rect.setFillColor({100, 100, 100, 255});
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(1);
    rect.setPosition(x, y);
    window.draw(rect);

    displayText(x + CELL_SIZE, y + CELL_SIZE * 1.5, CELL_SIZE, "Avion", window);
    displayText(x + CELL_SIZE, y + CELL_SIZE * 3.5, CELL_SIZE,
                "Type : " + std::to_string(m_aircraft->getType()), window);
    displayText(x + CELL_SIZE, y + CELL_SIZE * 5, CELL_SIZE,
                "Capacite : " + std::to_string(m_aircraft->getTankCapacity()), window);
    displayText(x + CELL_SIZE, y + CELL_SIZE * 6.5, CELL_SIZE,
                "Consommation : " + std::to_string(m_aircraft->getConsumption()), window);

    sf::RectangleShape rect1;
    rect1.setSize(sf::Vector2f(200, 100));
    rect1.setFillColor({50, 50, 150, 255});
    rect1.setPosition(xvol, yvol);
    window.draw(rect1);


    displayText(xvol + 5, yvol + 5, CELL_SIZE * 1.5, "Plan de vol", window);
    displayText(xvol + 5, yvol + CELL_SIZE * 3.5, CELL_SIZE,
                "Aeroport de depart : " + m_departure->getName(), window);
    displayText(xvol + 5, yvol + CELL_SIZE * 5, CELL_SIZE,
                "Aeroport d'arrive : " + m_arrival->getName(), window);
    displayText(xvol + 5, yvol + CELL_SIZE * 6.5, CELL_SIZE,
                "Heure de depart : " + conversionUTHeure(m_departureTime), window);
}

void FlightsPlanning::displayPath(std::vector<Airport *> toDisplay) {

    for (int i = 0; i < toDisplay.size(); ++i) {
        std::cout << toDisplay[i]->getName();
        if (i != toDisplay.size() - 1) {
            std::cout << " --> ";
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void FlightsPlanning::lostFuel() {

}

void FlightsPlanning::setDeparture(Airport *departure) {
    m_departure = departure;
}

void FlightsPlanning::setArrival(Airport *arrival) {
    m_arrival = arrival;
}

void FlightsPlanning::setStep(std::vector<Airport *> steps) {
    m_steps = steps;
}

std::vector<Airport *> FlightsPlanning::getSteps() {
    return m_steps;
}

void FlightsPlanning::setPath(std::vector<Cell *> path) {
    m_path = path;
}

std::vector<Cell *> FlightsPlanning::getPath() {
    return m_path;
}

int FlightsPlanning::getDistance() {
    return m_distance;
}

void FlightsPlanning::setDistance(int newDistance) {
    m_distance = newDistance;
}

void FlightsPlanning::calculateDistance() {
    for (int i = 0; i < m_steps.size() - 1; ++i) {
        m_steps[i]->getDistanceTo(m_steps[i + 1]);
    }
}

void FlightsPlanning::autoFillOtherInfo() {
    m_currentAirport = m_departure;
    m_hasStarted = false;
    m_hasArrived = false;
    m_departureTime = randomInt(5, 40);
    m_nbSteps = m_steps.size();
    if (m_nbSteps == 2) {
        m_isDirect = true;
    } else m_isDirect = false;
}

Airport *FlightsPlanning::getDeparture() {
    return m_departure;
}

Airport *FlightsPlanning::getArrival() {
    return m_arrival;
}

void FlightsPlanning::clearSteps() {
    m_steps.clear();
}

Aircraft *FlightsPlanning::getAircraft() {
    return m_aircraft;
}

int FlightsPlanning::getCurrentStep() {
    return m_currentStep;
}

///Chercher la rotation de l'avion
long double FlightsPlanning::getRotation() {
    int x2 = m_steps[m_currentStep + 1]->getPos().first; // x2 = x arrivee
    int y2 = m_steps[m_currentStep + 1]->getPos().second; // y2 = y arrivee
    int x1 = m_aircraft->getPos().first; // x1 = x avion
    int y1 = m_aircraft->getPos().second; // y1 = y avion

    int coefDistance;
    long double coefRot;

    if (x1 < x2 && y1 > y2) {
        coefDistance = (x2 - x1) / (y1 - y2);
        coefRot = atan((long double) (coefDistance)) * 58;
    } else if (x1 < x2 && y1 < y2) {
        coefDistance = (y2 - y1) / (x2 - x1);
        coefRot = atan((long double) (coefDistance)) * 58 + 90;
    } else if (x1 >= x2 && y1 < y2) {
        coefDistance = (x1 - x2) / (y2 - y1);
        coefRot = atan((long double) (coefDistance)) * 58 + 180;
    } else if (x1 > x2 && y1 > y2) {
        coefDistance = (y1 - y2) / (x1 - x2);
        coefRot = atan((long double) (coefDistance)) * 58 + 270;
    }

    return coefRot;
}

bool FlightsPlanning::hasStarted() const {
    return m_hasStarted;
}

bool FlightsPlanning::hasArrived() const {
    return  m_hasArrived;
}

void FlightsPlanning::setHasStarted(bool mHasStarted) {
    m_hasStarted = mHasStarted;
}

void FlightsPlanning::setDepartureTime(int mDepartureTime) {
    m_departureTime = mDepartureTime;
}
