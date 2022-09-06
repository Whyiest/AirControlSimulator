#include "../HEADERS/Core.h"

///Constructeur
Core::Core() = default;

///Destructeur
Core::~Core() {
    clock_t start = clock();
    std::cout << "Closing application... "
              << (double) (clock() - start) / CLOCKS_PER_SEC
              << "s" << std::endl;
}

void Core::initialisation() {
    std::cout << "-------------------MENU-----------------" << std::endl
              << " 1 : Afficher les donnees des aeroports et des avions" << std::endl
              << " 2 : Afficher le monde" << std::endl
              << " 3 : Afficher le monde avec la simulation"<< std::endl;

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1 :
            m_network.loadData("../DATA/defaultConfig.txt");
            m_network.print();
            break;
        case 2 :
            m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Core", sf::Style::Close);
            m_window.setFramerateLimit(60);
            backgroundTexture.loadFromImage(newImage("../DATA/worldMap.png", WINDOW_WIDTH, WINDOW_HEIGHT));
            backgroundSprite.setTexture(backgroundTexture);
            m_network.loadData("../DATA/defaultConfig.txt");
            m_network.defineCellTypes();
            m_network.initiateFlightPlanning();
            loop(false);
            break;
        case 3 :
            std::cout << "Lancement de la simulation." << std::endl;
            m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Core", sf::Style::Close);
            m_window.setFramerateLimit(60);
            backgroundTexture.loadFromImage(newImage("../DATA/worldMap.png", WINDOW_WIDTH, WINDOW_HEIGHT));
            backgroundSprite.setTexture(backgroundTexture);
            m_network.loadData("../DATA/defaultConfig.txt");
            m_network.defineCellTypes();
            m_network.initiateFlightPlanning();
            loop(true);
            break;
        default:
            std::cout << "Veuillez saisir un choix valide." << std::endl;
            break;
    }
}

///Boucle de la simulation
void Core::loop(bool doUpdate) {
    sf::Clock clock;
    sf::Time elapsedTime = sf::Time::Zero;

    while (m_window.isOpen()) {
        elapsedTime += clock.restart();
        handleEvents();

        if (elapsedTime > sf::seconds(TIME_UNIT) && doUpdate) {
            elapsedTime = sf::Time::Zero;
            utPassed++;
            m_network.update(utPassed);
            m_network.defineCellTypes();
        }

        display();
    }
}

///Gère les évènements
void Core::handleEvents() {
    sf::Event event{};

    while (m_window.pollEvent(event)) {

        switch (event.type) {

            case sf::Event::Closed:
                m_window.close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) m_window.close();
                if (event.key.code == sf::Keyboard::Space) m_debug = !m_debug;
                break;

            case sf::Event::MouseMoved:
                m_mouseGridPos = {map(sf::Mouse::getPosition(m_window).x, 0, WINDOW_WIDTH, 0, GRID_WIDTH),
                                  map(sf::Mouse::getPosition(m_window).y, 0, WINDOW_HEIGHT, 0, GRID_HEIGHT)};
                break;
        }
    }
}

///Gère l'affichage
void Core::display() {
    if (!m_window.hasFocus()) return;

    m_window.clear();

    m_window.draw(backgroundSprite);

    m_network.display(m_debug, m_window, m_mouseGridPos);

    m_window.display();
}


