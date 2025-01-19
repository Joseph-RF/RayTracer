// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "vec3.h"
#include "sphere.h"
#include "pixelmap.h"
#include "ray.h"
#include "material.h"

#include <iostream>
#include <vector>
#include <memory>

bool raySphereIntersection(
	ray& r,
	std::shared_ptr<sphere>& s,
	double t_min,
	double t_max,
	double& t
);
colour traceRay(
	ray& r,
	std::vector<std::shared_ptr<sphere>>& sphere_list,
	double t_min,
	double t_max,
	int depth
);

double infinity = std::numeric_limits<double>::infinity();
double small_amount = 0.000001;
double max_depth = 10;

int main()
{
    const int width = 600;
    const int height = 600;

    sf::VideoMode video_mode(width, height);
    sf::RenderWindow window(video_mode, "RayTracer", sf::Style::Close);
	sf::Event e;

	// Define the camera and canvas
	vec3 O = vec3(0.0, 0.0, 0.0);
	double d = 1.0; // z-axis distance from camera to canvas
	pixelmap pm(width, height);

	int pixel_samples = 10;
	double pixel_sampling_factor = 1.0 / pixel_samples;

	// Add new materials
	std::shared_ptr<diffuse> red_diffuse = std::make_shared<diffuse>(
		colour(0.8, 0.1, 0.0));
	std::shared_ptr<diffuse> blue_diffuse = std::make_shared<diffuse>(
		colour(0.1, 0.0, 0.85));
	std::shared_ptr<metal> shiny_metal = std::make_shared<metal>(
		colour(0.9, 0.9, 0.9));
	std::shared_ptr<diffuse> yellow_diffuse = std::make_shared<diffuse>(
		colour(0.85, 0.85, 0.0));

	// Add objects to the scene
	std::vector<std::shared_ptr<sphere>> spheres;
	spheres.push_back(std::make_shared<sphere>(
		vec3(0.0, -1.0, 3.0), 1.0, red_diffuse));
	spheres.push_back(std::make_shared<sphere>(
		vec3(-2.0, 0.0, 4.0), 1.0, blue_diffuse));
	spheres.push_back(std::make_shared<sphere>(
		vec3(2.0, 0.0, 4.0), 1.0, shiny_metal));
	spheres.push_back(std::make_shared<sphere>(
		vec3(4.0, 0.0, 8.0), 2.0, red_diffuse));
	spheres.push_back(std::make_shared<sphere>(
		vec3(0.0, -5001.0, 0.0), 5000.0, yellow_diffuse));

	// Render the scene
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			colour pixel_colour;
			for (int k = 0; k < pixel_samples; ++k) {
				double v_x = ((i + random_num()) - width / 2.0) / (width) * 1.0;
				double v_y = ((j + random_num()) - height / 2.0) / (height) * 1.0;

				ray r(O, vec3(v_x, v_y, d));
				pixel_colour += traceRay(r, spheres, small_amount, infinity, 1);
			}

			pixel_colour *= pixel_sampling_factor;
			pixel_colour.clampColour();

			pm.modifypixel(i, j, pixel_colour);
		}
		std::cout << "Rows left: " << (height - j) << std::endl;
	}

	// Create the sprite that will be displayed on window
	sf::Image render_image;
	render_image.create(width, height, pm.pixels.data());

	sf::Texture texture;
	texture.loadFromImage(render_image);

	sf::Sprite window_sprite(texture);
	window_sprite.setOrigin(0.f, height);
	window_sprite.setScale(1.0f, -1.0f);

    while (window.isOpen()) {

		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (e.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				break;
			}
		}

        window.clear();

        window.draw(window_sprite);

        window.display();
    }
}

bool raySphereIntersection(
	ray& r,
	std::shared_ptr<sphere>& s,
	double t_min,
	double t_max,
	double& t
)
{
	// Returns false if no intersection

	double closest_t = infinity;

	vec3 OC = r.origin - s->position;
	double a = dot(r.direction, r.direction);
	double b = 2 * dot(OC, r.direction);
	double c = dot(OC, OC) - (s->radius * s->radius);

	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0.0) {
		return false;
	}

	double discriminant_sqrt = std::sqrt(discriminant);

	double t1 = (-b + discriminant_sqrt )/ (2 * a);
	double t2 = (-b - discriminant_sqrt) / (2 * a);

	if (t1 > t_min && t1 < t_max && t1 < closest_t) {
		closest_t = t1;
	}
	if (t2 > t_min && t2 < t_max && t2 < closest_t) {
		closest_t = t2;
	}
	t = closest_t;
	return true;
}

colour traceRay(
	ray& r,
	std::vector<std::shared_ptr<sphere>>& sphere_list,
	double t_min,
	double t_max,
	int depth
)
{
	if (depth >= max_depth) {
		return colour(0.0, 0.0, 0.0);
	}

	double closest_t = infinity;
	double t = 0.0;
	std::shared_ptr<sphere> closest_sphere = nullptr;

	for (std::shared_ptr<sphere> sphere : sphere_list) {
		if (raySphereIntersection(r, sphere, t_min, t_max, t)) {
			if (t < closest_t) {
				closest_t = t;
				closest_sphere = sphere;
			}
		}
	}
	
	if (closest_sphere == nullptr) {
		// Colour of the background / sky
		return colour(0.7, 0.9, 1.0);
	}
	
	ray scattered(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0));
	colour hit_colour;

	closest_sphere->hit(r, closest_t, scattered, hit_colour);

	return hit_colour * traceRay(scattered, sphere_list, t_min, t_max, depth + 1);
}
