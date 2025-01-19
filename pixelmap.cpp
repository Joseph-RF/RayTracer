#include "pixelmap.h"

pixelmap::pixelmap(const int nx, const int ny) : nx(nx), ny(ny)
{
    pixels = std::vector<sf::Uint8>(nx * ny * 4, 0);
}

void pixelmap::modifypixel(int x, int y, const colour& colour)
{
    int index = (nx * y + x) * 4; // Need to multiply by 4 due to RGBA

    if (index >= pixels.size()) {
        std::cout << "out of range" << std::endl;
    }

    pixels[index] = 255 * colour.get_x(); // Red
    pixels[index + 1] = 255 * colour.get_y(); // Green
    pixels[index + 2] = 255 * colour.get_z(); // Blue
    pixels[index + 3] = 255; // Alpha
}
