// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

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
#include "interval.h"
#include "bvhnode.h"

void spheresScene();
void cornellBox();
void singleBox();

void addCuboid(
	const Vec3& pos,
	const Vec3& size,
	const double y_rot,
	std::shared_ptr<Material> mat,
	std::vector<std::shared_ptr<Hittable>>& hittable_list
);

void render(int total_thread_count, int thread_num);

/*
* OLD TRACE RAY WITH LIST
Colour traceRay(
	Ray& r,
	std::vector<std::shared_ptr<Hittable>>& hittable_list,
	Interval t_range,
	int depth
);
*/

Colour traceRay(
	Ray& r,
	BVHNode& hittable_tree,
	Interval t_range,
	int depth
);

double max_depth = 10;

const int width = 800;
const int height = 800;


// Define the Camera
Camera cam;
PixelMap pm(width, height);

std::vector<std::shared_ptr<Hittable>> hittables;
BVHNode world;

int main() {
    sf::VideoMode video_mode(width, height);
    sf::RenderWindow window(video_mode, "RayTracer", sf::Style::Close);
	sf::Event e;

	auto start = std::chrono::system_clock::now();

	//spheresScene();
	cornellBox();
	//singleBox();

	// After creating objects, create BVH tree using the list
	world = BVHNode(hittables);

	// Render the scene

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

	auto end = std::chrono::system_clock::now();

	double time_elapsed = std::chrono::duration_cast<
		std::chrono::duration<double>>(end - start).count();
	std::cout << "Render time: " << time_elapsed << std::endl;

	//render(1, 0);

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

void spheresScene() {
	// Add materials
	std::shared_ptr<Metal> red_diffuse = std::make_shared<Metal>(
		Colour(0.8, 0.1, 0.0));
	std::shared_ptr<Diffuse> blue_diffuse = std::make_shared<Diffuse>(
		Colour(0.1, 0.0, 0.85));
	std::shared_ptr<Metal> shiny_metal = std::make_shared<Metal>(
		Colour(0.9, 0.9, 0.9));
	std::shared_ptr<Diffuse> yellow_diffuse = std::make_shared<Diffuse>(
		Colour(0.85, 0.85, 0.0));
	std::shared_ptr<Light> white_light = std::make_shared<Light>(
		Colour(20.0, 20.0, 20.0));

	// Add shapes
	std::shared_ptr<Sphere> red_sphere0_body = std::make_shared<Sphere>(
		Vec3(0.0, -1.0, 3.0), 1.0);
	std::shared_ptr<Sphere> blue_sphere0_body = std::make_shared<Sphere>(
		Vec3(-2.0, 0.0, 4.0), 1.0);
	std::shared_ptr<Sphere> metal_sphere0_body = std::make_shared<Sphere>(
		Vec3(2.0, 0.0, 4.0), 1.0);
	std::shared_ptr<Sphere> red_sphere1_body = std::make_shared<Sphere>(
		Vec3(4.0, 0.0, 8.0), 2.0);
	std::shared_ptr<Sphere> yellow_sphere0_body = std::make_shared<Sphere>(
		Vec3(0.0, -5001.0, 0.0), 5000.0);
	std::shared_ptr<Sphere> light_sphere0_body = std::make_shared<Sphere>(
		Vec3(-4.0, 10.0, -0.0), 1.0);
	//std::shared_ptr<Quad> red_Quad0_body = std::make_shared<Quad>(
	//	Vec3(-2.0, 1.0, 6.0), Vec3(0.0, 1.0, -1.0), Vec3(1.0, 0.0, 0.0));
	//std::shared_ptr<triangle> red_tri0_body = std::make_shared<triangle>(
	//	Vec3(-2.0, 1.0, 6.0), Vec3(0.0, 1.0, -1.0), Vec3(1.0, 0.0, 0.0));
	std::shared_ptr<Cube> red_cub0_body = std::make_shared<Cube>(
		Vec3(-2.0, -1.0, 0.0), 1.0);

	// Add objects to the scene
	hittables.push_back(std::make_shared<Hittable>(
		red_sphere0_body, red_diffuse));
	hittables.push_back(std::make_shared<Hittable>(
		blue_sphere0_body, blue_diffuse));
	hittables.push_back(std::make_shared<Hittable>(
		metal_sphere0_body, shiny_metal));
	hittables.push_back(std::make_shared<Hittable>(
		red_sphere1_body, red_diffuse));
	hittables.push_back(std::make_shared<Hittable>(
		yellow_sphere0_body, yellow_diffuse));
	hittables.push_back(std::make_shared<Hittable>(
		red_cub0_body, blue_diffuse));

	// Lights
	hittables.push_back(std::make_shared<Hittable>(
		light_sphere0_body, white_light));
}

void cornellBox() {
	std::shared_ptr<Diffuse> red = std::make_shared<Diffuse>(Colour(0.65, 0.05, 0.05));
	std::shared_ptr<Diffuse> white = std::make_shared<Diffuse>(Colour(0.73, 0.73, 0.73));
	std::shared_ptr<Diffuse> green = std::make_shared<Diffuse>(Colour(0.12, 0.45, 0.15));
	std::shared_ptr<Light> white_light = std::make_shared<Light>(Colour(15, 15, 15));
	std::shared_ptr<Metal> shiny_metal = std::make_shared<Metal>(Colour(0.8, 0.8, 0.8));
	std::shared_ptr<Dielectric> glass = std::make_shared<Dielectric>(Colour(1.0, 1.0, 1.0), 1.4);

	std::shared_ptr<Quad> wall0 = std::make_shared<Quad>(Vec3(2, 0, 0), Vec3(0, 2, 0), Vec3(0, 0, 2));
	std::shared_ptr<Quad> wall1 = std::make_shared<Quad>(Vec3(0, 0, 0), Vec3(0, 2, 0), Vec3(0, 0, 2));
	// world.add(make_shared<Quad>(point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
	std::shared_ptr<Quad> wall2 = std::make_shared<Quad>(Vec3(2, 2, 2), Vec3(-2, 0, 0), Vec3(0, 0, -2));
	// world.add(make_shared<Quad>(point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
	std::shared_ptr<Quad> wall3 = std::make_shared<Quad>(Vec3(0, 0, 0), Vec3(2, 0, 0), Vec3(0, 0, 2));
	std::shared_ptr<Quad> wall4 = std::make_shared<Quad>(Vec3(0, 0, 2), Vec3(2, 0, 0), Vec3(0, 2, 0));
	std::shared_ptr<Quad> light_body = std::make_shared<Quad>(Vec3(1.24, 1.99, 1.22), Vec3(-0.47, 0, 0), Vec3(0, 0, -0.38));

	std::shared_ptr<Sphere> left_sphere = std::make_shared<Sphere>(Vec3(0.5, 0.5, 1.0), 0.4);

	//addCuboid(Vec3(0.2, 0.0, 1.0), Vec3(0.5, 0.5, 0.5), 60.0, white, hittables);
	addCuboid(Vec3(1.5, 0.0, 0.5), Vec3(0.5, 0.5, 0.5), -45.0, white, hittables);

	hittables.push_back(std::make_shared<Hittable>(
		wall0, green));
	hittables.push_back(std::make_shared<Hittable>(
		wall1, red));
	hittables.push_back(std::make_shared<Hittable>(
		wall2, white));
	hittables.push_back(std::make_shared<Hittable>(
		wall3, white));
	hittables.push_back(std::make_shared<Hittable>(
		wall4, white));
	hittables.push_back(std::make_shared<Hittable>(
		light_body, white_light));
	hittables.push_back(std::make_shared<Hittable>(
		left_sphere, glass));
}

void singleBox() {
	std::shared_ptr<Diffuse> yellow_diffuse = std::make_shared<Diffuse>(
		Colour(0.85, 0.85, 0.0));
	std::shared_ptr<Diffuse> white = std::make_shared<Diffuse>(Colour(0.73, 0.73, 0.73));

	std::shared_ptr<Sphere> yellow_sphere0_body = std::make_shared<Sphere>(
	Vec3(0.0, -5001.0, 0.0), 5000.0);

	Vec3 u(1.0, 0.0, 0.0);
	Vec3 v(0.0, 1.0, 0.0);
	Vec3 w(0.0, 0.0, 1.0);
	double angle = 60.0;

	std::shared_ptr<Cube> block0 = std::make_shared<Cube>(
		Vec3(1.0, 0.0, 1.0),
		u.y_rotation(angle),
		v.y_rotation(angle),
		w.y_rotation(angle),
		0.7);

	hittables.push_back(std::make_shared<Hittable>(
		yellow_sphere0_body, yellow_diffuse));
	hittables.push_back(std::make_shared<Hittable>(
		block0, white));
}

void addCuboid(
	const Vec3& pos,
	const Vec3& size,
	const double y_rot,
	std::shared_ptr<Material> mat,
	std::vector<std::shared_ptr<Hittable>>& hittable_list) {

	const Vec3 dx = Vec3(size.get_x(), 0.0, 0.0).y_rotation(y_rot);
	const Vec3 dy = Vec3(0.0, size.get_y(), 0.0).y_rotation(y_rot);
	const Vec3 dz = Vec3(0.0, 0.0, size.get_z()).y_rotation(y_rot);

	std::shared_ptr<Quad> face0 = std::make_shared<Quad>(pos, dx, dy);
	std::shared_ptr<Quad> face1 = std::make_shared<Quad>(pos, dx, dz);
	std::shared_ptr<Quad> face2 = std::make_shared<Quad>(pos, dz, dy);
	std::shared_ptr<Quad> face3 = std::make_shared<Quad>(pos + dz, dx, dy);
	std::shared_ptr<Quad> face4 = std::make_shared<Quad>(pos + dy, dx, dz);
	std::shared_ptr<Quad> face5 = std::make_shared<Quad>(pos + dx, dz, dy);

	hittable_list.push_back(std::make_shared<Hittable>(face0, mat));
	hittable_list.push_back(std::make_shared<Hittable>(face1, mat));
	hittable_list.push_back(std::make_shared<Hittable>(face2, mat));
	hittable_list.push_back(std::make_shared<Hittable>(face3, mat));
	hittable_list.push_back(std::make_shared<Hittable>(face4, mat));
	hittable_list.push_back(std::make_shared<Hittable>(face5, mat));
}

void render(int total_thread_count, int thread_num) {
	// Render the scene
	for (int j = thread_num; j < height; j += total_thread_count) {
		for (int i = 0; i < width; ++i) {
			Colour pixel_Colour;
			for (int k = 0; k < cam.pixel_samples; ++k) {
				double v_x = ((i + random_num()) - width / 2.0) / (width) * 1.0;
				double v_y = ((j + random_num()) - height / 2.0) / (height) * 1.0;

				Ray r(cam.position, v_x * cam.u + v_y * cam.v + cam.d * cam.w);
				pixel_Colour += traceRay(r, world, Interval(small, big), 1);
			}

			pixel_Colour *= cam.pixel_sampling_factor;

			pm.modifypixel(i, j, pixel_Colour);
		}
		std::cout << "Rows left: " << (height - j) << std::endl;
	}
}

Colour traceRay(
	Ray& r,
	BVHNode& hittable_tree,
	Interval t_range,
	int depth) {

	if (depth >= max_depth) {
		return Colour(0.0, 0.0, 0.0);
	}

	double closest_t = big;
	double t = 0.0;
	std::shared_ptr<Hittable> closest_hittable = nullptr;
	
	/*
	for (std::shared_ptr<Hittable> hittable : hittables) {
		if (hittable->hit(r, t_range, t)) {
			if (t < closest_t) {
				closest_t = t;
				closest_hittable = hittable;
			}
		}
	}
	*/
	
	closest_hittable = hittable_tree.hit(r, t_range, closest_t);

	if (closest_hittable == nullptr) {
		// Colour of the background / sky
		//return Colour(0.0, 0.0, 0.0);
		return Colour(0.7, 0.9, 1.0) * 0.8;
		//return Colour(0.1, 0.1, 0.1);
	}
	
	Ray scattered(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0));
	Colour hit_colour;

	if (!closest_hittable->scatter(r, closest_t, scattered, hit_colour)) {
		return hit_colour;
	}

	return hit_colour * traceRay(scattered, hittable_tree, t_range, depth + 1);
}
