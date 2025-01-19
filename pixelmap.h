#pragma once

#include "SFML/System.hpp"

#include "vec3.h"

#include <iostream>
#include <vector>

class pixelmap
{
public:
    pixelmap(int nx, int ny);

    std::vector<sf::Uint8> pixels;

    void modifypixel(int x, int y, const colour& colour);

private:
    int nx;
    int ny;
};

