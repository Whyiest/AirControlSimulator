#ifndef CONTROLEAERIEN_GROUPE_2_G_LIBRARIES_H
#define CONTROLEAERIEN_GROUPE_2_G_LIBRARIES_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <random>
#include <utility>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>


#define TIME_UNIT 0.1 ///METTRE A 2.0 AVANT RENDU ET SOUTENANCE

#define WINDOW_HEIGHT 800.0 ///VALEUR A MODIFIER POUR CHANGER LA TAILLE DE LA FENETRE
#define WINDOW_WIDTH (WINDOW_HEIGHT * 1.8)
#define CELL_SIZE (WINDOW_HEIGHT / 80)
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

typedef std::pair<int, int> Pair;
typedef std::tuple<double, int, int> Tuple;

#endif
