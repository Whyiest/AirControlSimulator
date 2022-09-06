#ifndef CONTROLEAERIEN_GROUPE_2_G_FLIGHTSPLANNING_H
#define CONTROLEAERIEN_GROUPE_2_G_FLIGHTSPLANNING_H

#include "Libraries.h"
#include "Airport.h"
#include "Aircraft.h"

class FlightsPlanning {

private:

    bool m_isDirect;

    bool m_hasStarted;

    bool m_hasArrived;

    Airport* m_departure;

    Airport* m_arrival;

    std::vector<Airport*> m_steps;

    std::vector<Cell *> m_path;

    Airport* m_currentAirport;

    int m_currentStep;

    int m_distance;

    int m_nbSteps;

    Aircraft* m_aircraft;

    int m_departureTime;

public:

    FlightsPlanning(Aircraft* aircraft);

    ~FlightsPlanning();

    void update(std::vector<std::vector<Cell*>> &cells, int ut);

    std::vector<Airport*> dijkstra(const std::vector<Airport*>& airportList);

    void Astar(std::vector<std::vector<Cell*>> &cells, int ut);

    void display(sf::RectangleShape &rect, sf::RenderWindow &window, bool debug, Pair mouseGridPos);

    void displayInfosAircraft(sf::RenderWindow &window);

    void setDeparture(Airport* departure);

    void setArrival(Airport* arrival);

    void setStep(std::vector<Airport *> steps);

    std::vector<Airport*> getSteps();

    void setPath(std::vector<Cell *> path);

    void lostFuel();

    std::vector<Cell *> getPath();

    void setDistance(int newDistance);

    int getDistance();

    void calculateDistance();

    void autoFillOtherInfo();

    Airport *getDeparture();

    Airport *getArrival();

    void displayPath (std::vector<Airport*>);

    void clearSteps();

    Aircraft* getAircraft();

    int getCurrentStep();

    long double getRotation();

    std::vector<std::pair<Airport *, int>> dijkstraInFlight(const std::vector<Airport *> &airportList);

    Airport *foundAirportByID(int thisID, const std::vector<Airport *> &airportList);

    void setCurrentStep(int newStep);

    bool hasStarted() const;

    bool hasArrived() const;

    void setHasStarted(bool mHasStarted);

    void setDepartureTime(int mDepartureTime);

    void addStep(Airport *newStep);

    void crash();
};

#endif
