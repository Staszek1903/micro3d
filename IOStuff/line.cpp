#include "line.h"



stuff::Line::Line()
{
    setFillColor(sf::Color::White);
}

stuff::Line::Line(const sf::Vector2f &_begining, const sf::Vector2f &_end)
    :begining(_begining), end(_end)
{
    setFillColor(sf::Color::White);
    update_shape();
}

sf::Vector2f stuff::Line::getBegining() const
{
    return begining;
}

void stuff::Line::setBegining(const sf::Vector2f &value)
{
    begining = value;
    update_shape();
}

sf::Vector2f stuff::Line::getEnd() const
{
    return end;
}

void stuff::Line::setEnd(const sf::Vector2f &value)
{
    end = value;
    update_shape();
}

void stuff::Line::setVector(const sf::Vector2f &value)
{
    end = begining + value;
    update_shape();
}

float stuff::Line::getLenght()
{
    auto vect = begining - end;
    float lenght = Vect::lenght(vect);
    return lenght;
}

float stuff::Line::getThickness() const
{
    return thickness;
}

void stuff::Line::setThickness(float value)
{
    thickness = value;
    update_shape();
}

void stuff::Line::update_shape()
{
    float lenght = getLenght();
    sf::Vector2f size(lenght, thickness);
    auto vect = end - begining;
    auto origin = size/2.0f;
    auto pos = begining + (vect/2.0f);
    double rot = Vect::getRotationDegrees(vect);

    setSize(size);
    setOrigin(origin);
    setPosition(pos);
    setRotation(static_cast<float>(rot));
}
