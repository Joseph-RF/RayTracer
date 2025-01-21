#pragma once

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "material.h"

class hittable
{
public:
	hittable(
		const vec3& position,
		const double radius,
		std::shared_ptr<material> mat
	);

	bool hit(ray& r, double t_min, double t_max, double& t);
	bool scatter(
		ray& r_in,
		double t,
		ray& r_out,
		colour& attenuation
	);

private:
	sphere body;
	std::shared_ptr<material> mat;
};