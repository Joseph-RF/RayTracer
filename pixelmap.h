#pragma once

#include <iostream>
#include <vector>

#include "SFML/System.hpp"

#include "vec3.h"

class pixelmap
{
public:
    pixelmap(int nx, int ny);

    std::vector<sf::Uint8> pixels;

    void modifypixel(int x, int y, const colour& pixel_colour);

private:
    int nx;
    int ny;
};

