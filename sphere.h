#pragma once

#include "vec3.h"
#include "material.h"

#include <memory>

class material;

class sphere
{
public:
	sphere();
	sphere(
		vec3 position,
		double radius,
		std::shared_ptr<material> mat
	);

	void hit(ray& r_in, double t, ray& r_out, colour& attenuation);

	vec3 position;
	double radius;
	std::shared_ptr<material> mat;
};
