#pragma once

#include "vec3.h"
#include "ray.h"
#include "shape.h"
#include "material.h"

class hittable
{
public:
	hittable(
		std::shared_ptr<shape> body,
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
	std::shared_ptr<shape> body;
	std::shared_ptr<material> mat;
};