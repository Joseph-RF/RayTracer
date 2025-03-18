#pragma once

#include "vec3.h"
#include "ray.h"
#include "shape.h"
#include "material.h"
#include "interval.h"

class Hittable
{
public:
	Hittable(
		std::shared_ptr<Shape> body,
		std::shared_ptr<Material> mat
	);

	bool hit(Ray& r, Interval t_range, double& t);
	bool scatter(Ray& r_in, double t, Ray& r_out, Colour& attenuation);

private:
	std::shared_ptr<Shape> body;
	std::shared_ptr<Material> mat;
};