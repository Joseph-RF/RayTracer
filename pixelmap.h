#pragma once

#include <iostream>
#include <vector>

#include "SFML/System.hpp"

#include "vec3.h"

class PixelMap
{
public:
    PixelMap(int nx, int ny);

    std::vector<sf::Uint8> pixels;

    void modifypixel(int x, int y, const Colour& pixel_colour);

private:
    int nx;
    int ny;
};

