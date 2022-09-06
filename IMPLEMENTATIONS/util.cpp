#include "../HEADERS/util.h"

///Renvoi un entier entre min et max inclus
int randomInt(int min, int max) {
    static std::mt19937 randGen(time(nullptr));
    return std::uniform_int_distribution<>(min, max)(randGen);
}

///map une valeur d'un interval dans un nouvel interval
float map(float n, float min1, float max1, float min2, float max2) {
    return min2 + (max2 - min2) * ((n - min1) / (max1 - min1));
}

///vérifie si des coordonnées se trouvent dans une zone rectangulaire défini par un coin en haut à gauche et un coin en bas à droite
bool inbound(int x, int y, int upperLeftX, int upperLeftY, int downRightX, int downRightY) {
    return x >= upperLeftX && x < downRightX && y >= upperLeftY && y < downRightY;
}

///Redimensionne une image (Proviens de la documention orignale de SMFL https://en.sfml-dev.org/forums/index.php?topic=24954.0)
void resizeImage(const sf::Image &originalImage, sf::Image &resizedImage) {
    const sf::Vector2u originalImageSize{originalImage.getSize()};
    const sf::Vector2u resizedImageSize{resizedImage.getSize()};
    for (unsigned int y{0u}; y < resizedImageSize.y; ++y) {
        for (unsigned int x{0u}; x < resizedImageSize.x; ++x) {
            unsigned int origX{
                    static_cast<unsigned int>(static_cast<double>(x) / resizedImageSize.x * originalImageSize.x)};
            unsigned int origY{
                    static_cast<unsigned int>(static_cast<double>(y) / resizedImageSize.y * originalImageSize.y)};
            resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
        }
    }
}

///Créer une nouvelle image directement redimensionnée
sf::Image newImage(const std::string &path, int resizedWidth, int resizedHeight) {
    sf::Image originalImage;
    sf::Image resizedImage;
    resizedImage.create(resizedWidth, resizedHeight);
    if (!originalImage.loadFromFile(path)) return sf::Image();
    resizeImage(originalImage, resizedImage);
    return resizedImage;
}

void drawLine(float x1, float y1, float x2, float y2, float thickness, sf::Color color, sf::RenderWindow &window) {
    sf::Vector2f point1 = {x1, y1};
    sf::Vector2f point2 = {x2, y2};
    sf::Vector2f direction = point2 - point1;
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

    sf::Vertex line[] = {
            sf::Vertex(point1 + offset, color),
            sf::Vertex(point2 + offset, color),
            sf::Vertex(point2 - offset, color),
            sf::Vertex(point1 - offset, color)
    };

    window.draw(line, 4, sf::Quads);
}

void displayText(float startX, float startY, float size, const std::string &textContent, sf::RenderWindow &window) {

    ///******************************************************************************************************//
    /// Permet d'afficher un texte simplement                                                                //
    ///*****************************************************************************************************//

    sf::Font font;
    sf::Text text;

    if (!font.loadFromFile("../DATA/LEMONMILK-Regular.otf")) {
        std::cerr << "Unable to open font";
        return;
    }

    text.setFont(font);
    text.setString(textContent);
    text.setCharacterSize(size);
    text.setPosition(startX, startY);
    window.draw(text);
}

std::string conversionUTHeure(int nbUT){
    int minutes;
    int heures;
    int totalminutes;

    totalminutes = nbUT*15;

    if(totalminutes > 1440){
        totalminutes = totalminutes - 1440;
    }

    minutes = totalminutes%60;
    heures = totalminutes - totalminutes%60;
    heures = heures/60;

    return std::to_string(heures) + "h" + std::to_string(minutes);
}