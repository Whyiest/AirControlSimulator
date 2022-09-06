#ifndef CONTROLEAERIEN_GROUPE_2_G_UTIL_H
#define CONTROLEAERIEN_GROUPE_2_G_UTIL_H

#include "Libraries.h"

int randomInt(int min, int max);


float map(float n, float min1, float max1, float min2, float max2);

bool inbound(int x, int y, int upperLeftX, int upperLeftY, int downRightX, int downRightY);

void resizeImage(const sf::Image &originalImage, sf::Image &resizedImage);

sf::Image newImage(const std::string& path, int resizedWidth, int resizedHeight);

void drawLine(float x1, float y1, float x2, float y2, float thickness, sf::Color color, sf::RenderWindow &window);

void displayText(float startX, float startY, float size, const std::string& textContent, sf::RenderWindow &window);

std::string conversionUTHeure(int nbUT);

#endif
