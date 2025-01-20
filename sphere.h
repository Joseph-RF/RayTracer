#pragma once

#include "vec3.h"
#include "material.h"

#include <memory>

class material;

class sphere
{
public:
	sphere();
	sphere(const vec3& position, const double radius);

	bool hit(ray& r, double t_min, double t_max, double& t);

	vec3 position;
	double radius;
};
