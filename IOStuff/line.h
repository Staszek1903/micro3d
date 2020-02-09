#ifndef LINE_H
#define LINE_H

#include <SFML/Graphics.hpp>
#include "vect.h"

namespace stuff {
    class Line : public sf::RectangleShape
    {
        sf::Vector2f begining, end; // global points of line;
        float thickness = 1.0f;

    public:
        Line();
        Line(const sf::Vector2f & _beggining, const sf::Vector2f & _end);



        sf::Vector2f getBegining() const;
        void setBegining(const sf::Vector2f &value);

        sf::Vector2f getEnd() const;
        void setEnd(const sf::Vector2f &value);

        void setVector(const sf::Vector2f &value);
        float getLenght();

        float getThickness() const;
        void setThickness(float value);

    private:
        void update_shape();
    };
}


#endif // LINE_H
