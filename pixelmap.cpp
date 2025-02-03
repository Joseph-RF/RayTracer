#include "pixelmap.h"

PixelMap::PixelMap(const int nx, const int ny) : nx(nx), ny(ny) {
    pixels = std::vector<sf::Uint8>(nx * ny * 4, 0);
}

void PixelMap::modifypixel(int x, int y, const Colour& pixel_colour) {
    int index = (nx * y + x) * 4; // Need to multiply by 4 due to RGBA

    if (index >= pixels.size()) {
        std::cout << "out of range" << std::endl;
    }

    Colour corrected_colour = pixel_colour;
    corrected_colour.clampColour();

    pixels[index] = 255.0 * gammaCorrection(corrected_colour.get_x()); // Red
    pixels[index + 1] = 255.0 * gammaCorrection(corrected_colour.get_y()); // Green
    pixels[index + 2] = 255.0 * gammaCorrection(corrected_colour.get_z()); // Blue
    pixels[index + 3] = 255.0; // Alpha
}
