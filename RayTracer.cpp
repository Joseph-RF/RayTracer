// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <memory>
#include <thread>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "vec3.h"
#include "shape.h"
#include "pixelmap.h"
#include "ray.h"
#include "material.h"
#include "hittable.h"
#include "camera.h"

void spheresScene();
void cornellBox();
void singleBox();

void render(int totalThreadCount, int threadNum);

colour traceRay(
	ray& r,
	std::vector<std::shared_ptr<hittable>>& hittable_list,
	double t_min,
	double t_max,
	int depth
);

double infinity = std::numeric_limits<double>::infinity();
double small_amount = 0.000001;
double max_depth = 10;

const int width = 800;
const int height = 800;


// Define the camera
camera cam;
pixelmap pm(width, height);

std::vector<std::shared_ptr<hittable>> hittables;

int main()
{
    sf::VideoMode video_mode(width, height);
    sf::RenderWindow window(video_mode, "RayTracer", sf::Style::Close);
	sf::Event e;

	//spheresScene();
	cornellBox();
	//singleBox();

	// Render the scene

	/*
	int threadCount = 6;
	std::thread worker_0(render, threadCount, 0);
	std::thread worker_1(render, threadCount, 1);
	std::thread worker_2(render, threadCount, 2);
	std::thread worker_3(render, threadCount, 3);
	std::thread worker_4(render, threadCount, 4);
	std::thread worker_5(render, threadCount, 5);

	worker_0.join();
	worker_1.join();
	worker_2.join();
	worker_3.join();
	worker_4.join();
	worker_5.join();
	*/

	render(1, 0);

	// Create the sprite that will be displayed on window
	sf::Image render_image;
	render_image.create(width, height, pm.pixels.data());
	render_image.saveToFile("render.png");

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

void spheresScene()
{
	// Add materials
	std::shared_ptr<metal> red_diffuse = std::make_shared<metal>(
		colour(0.8, 0.1, 0.0));
	std::shared_ptr<diffuse> blue_diffuse = std::make_shared<diffuse>(
		colour(0.1, 0.0, 0.85));
	std::shared_ptr<metal> shiny_metal = std::make_shared<metal>(
		colour(0.9, 0.9, 0.9));
	std::shared_ptr<diffuse> yellow_diffuse = std::make_shared<diffuse>(
		colour(0.85, 0.85, 0.0));
	std::shared_ptr<light> white_light = std::make_shared<light>(
		colour(20.0, 20.0, 20.0));

	// Add shapes
	std::shared_ptr<sphere> red_sphere0_body = std::make_shared<sphere>(
		vec3(0.0, -1.0, 3.0), 1.0);
	std::shared_ptr<sphere> blue_sphere0_body = std::make_shared<sphere>(
		vec3(-2.0, 0.0, 4.0), 1.0);
	std::shared_ptr<sphere> metal_sphere0_body = std::make_shared<sphere>(
		vec3(2.0, 0.0, 4.0), 1.0);
	std::shared_ptr<sphere> red_sphere1_body = std::make_shared<sphere>(
		vec3(4.0, 0.0, 8.0), 2.0);
	std::shared_ptr<sphere> yellow_sphere0_body = std::make_shared<sphere>(
		vec3(0.0, -5001.0, 0.0), 5000.0);
	std::shared_ptr<sphere> light_sphere0_body = std::make_shared<sphere>(
		vec3(-4.0, 10.0, -0.0), 1.0);
	//std::shared_ptr<quad> red_quad0_body = std::make_shared<quad>(
	//	vec3(-2.0, 1.0, 6.0), vec3(0.0, 1.0, -1.0), vec3(1.0, 0.0, 0.0));
	//std::shared_ptr<triangle> red_tri0_body = std::make_shared<triangle>(
	//	vec3(-2.0, 1.0, 6.0), vec3(0.0, 1.0, -1.0), vec3(1.0, 0.0, 0.0));
	std::shared_ptr<cube> red_cub0_body = std::make_shared<cube>(
		vec3(-2.0, -1.0, 0.0), 1.0);

	// Add objects to the scene
	hittables.push_back(std::make_shared<hittable>(
		red_sphere0_body, red_diffuse));
	hittables.push_back(std::make_shared<hittable>(
		blue_sphere0_body, blue_diffuse));
	hittables.push_back(std::make_shared<hittable>(
		metal_sphere0_body, shiny_metal));
	hittables.push_back(std::make_shared<hittable>(
		red_sphere1_body, red_diffuse));
	hittables.push_back(std::make_shared<hittable>(
		yellow_sphere0_body, yellow_diffuse));
	hittables.push_back(std::make_shared<hittable>(
		red_cub0_body, blue_diffuse));

	// Lights
	hittables.push_back(std::make_shared<hittable>(
		light_sphere0_body, white_light));
}

void cornellBox()
{
	std::shared_ptr<diffuse> red = std::make_shared<diffuse>(colour(0.65, 0.05, 0.05));
	std::shared_ptr<diffuse> white = std::make_shared<diffuse>(colour(0.73, 0.73, 0.73));
	std::shared_ptr<diffuse> green = std::make_shared<diffuse>(colour(0.12, 0.45, 0.15));
	std::shared_ptr<light> white_light = std::make_shared<light>(colour(15, 15, 15));
	std::shared_ptr<metal> shiny_metal = std::make_shared<metal>(colour(0.8, 0.8, 0.8));

	std::shared_ptr<quad> wall0 = std::make_shared<quad>(vec3(2, 0, 0), vec3(0, 2, 0), vec3(0, 0, 2));
	std::shared_ptr<quad> wall1 = std::make_shared<quad>(vec3(0, 0, 0), vec3(0, 2, 0), vec3(0, 0, 2));
	// world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	std::shared_ptr<quad> wall2 = std::make_shared<quad>(vec3(2, 2, 2), vec3(-2, 0, 0), vec3(0, 0, -2));
	// world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
	std::shared_ptr<quad> wall3 = std::make_shared<quad>(vec3(0, 0, 0), vec3(2, 0, 0), vec3(0, 0, 2));
	std::shared_ptr<quad> wall4 = std::make_shared<quad>(vec3(0, 0, 2), vec3(2, 0, 0), vec3(0, 2, 0));
	std::shared_ptr<quad> light_body = std::make_shared<quad>(vec3(1.24, 1.99, 1.22), vec3(-0.47, 0, 0), vec3(0, 0, -0.38));

	vec3 u(1.0, 0.0, 0.0);
	vec3 v(0.0, 1.0, 0.0);
	vec3 w(0.0, 0.0, 1.0);
	double angle = 60.0;

	std::shared_ptr<cube> block0 = std::make_shared<cube>(
		vec3(0.2, 0.0, 1.0),
		u.y_rotation(angle),
		v.y_rotation(angle),
		w.y_rotation(angle),
		0.5);

	angle = -45.0;

	std::shared_ptr<cube> block1 = std::make_shared<cube>(
		vec3(1.5, 0.0, 0.5),
		u.y_rotation(angle),
		v.y_rotation(angle),
		w.y_rotation(angle),
		0.5);

	hittables.push_back(std::make_shared<hittable>(
		wall0, red));
	hittables.push_back(std::make_shared<hittable>(
		wall1, green));
	hittables.push_back(std::make_shared<hittable>(
		wall2, white));
	hittables.push_back(std::make_shared<hittable>(
		wall3, white));
	hittables.push_back(std::make_shared<hittable>(
		wall4, white));
	hittables.push_back(std::make_shared<hittable>(
		light_body, white_light));
	hittables.push_back(std::make_shared<hittable>(
		block0, white));
	hittables.push_back(std::make_shared<hittable>(
		block1, shiny_metal));
}

void singleBox()
{
	std::shared_ptr<diffuse> yellow_diffuse = std::make_shared<diffuse>(
		colour(0.85, 0.85, 0.0));
	std::shared_ptr<diffuse> white = std::make_shared<diffuse>(colour(0.73, 0.73, 0.73));

	std::shared_ptr<sphere> yellow_sphere0_body = std::make_shared<sphere>(
	vec3(0.0, -5001.0, 0.0), 5000.0);

	vec3 u(1.0, 0.0, 0.0);
	vec3 v(0.0, 1.0, 0.0);
	vec3 w(0.0, 0.0, 1.0);
	double angle = 60.0;

	std::shared_ptr<cube> block0 = std::make_shared<cube>(
		vec3(1.0, 0.0, 1.0),
		u.y_rotation(angle),
		v.y_rotation(angle),
		w.y_rotation(angle),
		0.7);

	hittables.push_back(std::make_shared<hittable>(
		yellow_sphere0_body, yellow_diffuse));
	hittables.push_back(std::make_shared<hittable>(
		block0, white));
}

void render(int totalThreadCount, int threadNum)
{
	// Render the scene
	for (int j = threadNum; j < height; j += totalThreadCount) {
		for (int i = 0; i < width; ++i) {
			colour pixel_colour;
			for (int k = 0; k < cam.pixel_samples; ++k) {
				double v_x = ((i + random_num()) - width / 2.0) / (width) * 1.0;
				double v_y = ((j + random_num()) - height / 2.0) / (height) * 1.0;

				ray r(cam.position, v_x * cam.u + v_y * cam.v + cam.d * cam.w);
				pixel_colour += traceRay(r, hittables, small_amount, infinity, 1);
			}

			pixel_colour *= cam.pixel_sampling_factor;

			pm.modifypixel(i, j, pixel_colour);
		}
		std::cout << "Rows left: " << (height - j) << std::endl;
	}
}

colour traceRay(
	ray& r,
	std::vector<std::shared_ptr<hittable>>& hittable_list,
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
	std::shared_ptr<hittable> closest_hittable = nullptr;

	for (std::shared_ptr<hittable> hittable : hittable_list) {
		if (hittable->hit(r, t_min, t_max, t)) {
			if (t < closest_t) {
				closest_t = t;
				closest_hittable = hittable;
			}
		}
	}

	if (closest_hittable == nullptr) {
		// Colour of the background / sky
		return colour(0.0, 0.0, 0.0);
		//return colour(0.7, 0.9, 1.0) * 0.8;
		//return colour(0.1, 0.1, 0.1);
	}
	
	ray scattered(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0));
	colour hit_colour;

	if (!closest_hittable->scatter(r, closest_t, scattered, hit_colour)) {
		return hit_colour;
	}

	return hit_colour * traceRay(scattered, hittable_list, t_min, t_max, depth + 1);
}
